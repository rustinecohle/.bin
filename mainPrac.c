#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define MAX 100
#define FILE_NAME "library.txt"
#define TEMP_NAME "library.tmp"
typedef struct {
    char branch[30];
    char ISBN [21];
    char title [101];
    char author [51];
    int copy;
    int year;
    float price;
} Book;

// readInt: reads one int. Letters give -1 (so the menu does not loop forever).
int readInt(void){
    int x;
    int r = scanf("%d", &x);
    if (r == EOF){
        printf("input ended\n");
        exit(0);
    }
    if (r != 1){
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);
        return -1;
    }
    return x;
}

// I chose to REPLACE any | the user types with - (instead of asking again),
// because the file format uses | as separator and this never breaks the file.
void replaceBar(char *s){
    for (; *s; s++){
        if (*s == '|') *s = '-';
    }
}

void promptBookDetails(Book *bookPtr){
    printf("enter isbn\n");
    scanf("%20s", bookPtr->ISBN);
    printf("enter the branch\n");
    scanf("%29s", bookPtr->branch);
    printf("enter title \n");
    scanf(" %100[^\n]",  bookPtr->title);
    printf("enter author name\n");
    scanf(" %50[^\n]", bookPtr->author);
    printf("enter copies available\n");
    scanf("%d", &bookPtr->copy);
    printf("ennter publish year\n");
    scanf("%d", &bookPtr->year);
    printf("enter price with 2 dcimals\n");
    scanf("%f", &bookPtr->price);
    replaceBar(bookPtr->ISBN);
    replaceBar(bookPtr->branch);
    replaceBar(bookPtr->title);
    replaceBar(bookPtr->author);
}
void displayBook(const Book *bookPtr){

    printf("isbn is %s\n", bookPtr->ISBN);
    printf("branch is %s\n", bookPtr->branch);
    printf("title  %s\n", bookPtr->title);

    printf("authir name is  %s\n", bookPtr->author);

    printf("copies available are %d\n", bookPtr->copy);

    printf("published year  %d\n", bookPtr->year);
    printf("price into 2 decimal points are %.2f\n", bookPtr->price);

}
int isDuplicate(const Book library[], int currentSize, const char *isbn, const char *branch){
    for (int i=0; i< currentSize; i++){
        if ((strcmp(library[i].ISBN, isbn))==0
        && (strcmp(library[i].branch, branch))==0){
            return 1;
        }

    } return 0;
}

// search by author: exact match, capital letters matter (case sensitive)
int findBooksByAuthor(Book library[], int currentSize, const char *author,
    Book *results[], int maxResults){
        int count=0;
        for(int i=0; i<currentSize && count<maxResults; i++){
            if((strcmp(library[i].author, author))==0){
                results[count]=&library[i];
                count++;
            }
        }


    return count;

}
int getRestockList(const Book library[], int currentSize, int minCopies) {
    int count = 0;
    for (int i = 0; i < currentSize; i++) {          // int i
        if (library[i].copy < minCopies) {
            printf("%d.\n", count + 1);              // running number
            displayBook(&library[i]);
            count++;
        }
    }
    if (count == 0) {                                // added
        printf("No books need restocking.\n");
    }
    return count;
}

int getOldBooksList(const Book library[], int currentSize, int currentYear, int minAge) {
    int count = 0;
    for (int i = 0; i < currentSize; i++) {
        if (currentYear - library[i].year > minAge) {   // > not <
            displayBook(&library[i]);
            count++;
        }
    }
    if (count == 0) {                                // added
        printf("No old books found.\n");
    }
    return count;
}
int saveLibraryToFile(const Book library[], int currentSize,const char *filename){
    FILE *fp=fopen(filename, "w");
    if(fp==NULL){
        printf("Error: could not open file.\n");
        return -1;}
    int count=0;
    for(int i=0; i<currentSize; i++){
        fprintf(fp,"%s|%s|%s|%s|%d|%d|%.2f\n",library[i].ISBN, library[i].branch, library[i].title, library[i].author, library[i].copy, library[i].year, library[i].price);

        count++;
    }
    fclose(fp);
    printf("Saved %d book(s) to %s.\n", count, filename);
    return count;


}
int loadLibraryFromFile(Book library[], int maxSize, const char *filename){
    FILE *fp=fopen(filename, "r");
    if(fp==NULL){
        printf("Error: could not open file.\n");
        return -1;
    }
    char line[500];
    int count=0;
    int lineNo=0;
    while(fgets(line, sizeof line, fp)!=NULL){
        lineNo++;
        if(count>=maxSize){
            printf("Warning: library full, remaining lines ignored.\n");
            break;
        }
        Book temp;
        int r = sscanf(line, "%20[^|]|%29[^|]|%100[^|]|%50[^|]|%d|%d|%f",
                temp.ISBN, temp.branch, temp.title, temp.author,
                &temp.copy, &temp.year, &temp.price);
        if(r!=7){
            printf("Warning: skipping bad line %d.\n", lineNo);
            continue;
        }
        library[count]=temp;
        count++;
    }
    fclose(fp);
    printf("Loaded %d book(s) from %s.\n", count, filename);
    return count;
}

// ---------- Question 8: update and delete ----------
int findBookIndex(const Book library[], int currentSize, const char *isbn, const char *branch){
    for(int i=0; i<currentSize; i++){
        if(strcmp(library[i].ISBN, isbn)==0
        && strcmp(library[i].branch, branch)==0){
            return i;
        }
    }
    return -1;
}
int updateCopies(Book library[], int currentSize, const char *isbn, const char *branch, int newCopies){
    if(newCopies<0) return 0;
    int index=findBookIndex(library, currentSize, isbn, branch);
    if(index==-1) return 0;
    library[index].copy=newCopies;
    return 1;
}
// currentSize is a pointer here because this function must change it
int deleteBook(Book library[], int *currentSize, const char *isbn, const char *branch){
    int index=findBookIndex(library, *currentSize, isbn, branch);
    if(index==-1) return 0;
    for(int i=index; i<*currentSize-1; i++){
        library[i]=library[i+1];                 // shift left
    }
    (*currentSize)--;
    return 1;
}
// returns 1 changed, 0 not found, -1 file problem
int updateCopiesInFile(const char *filename, const char *isbn, const char *branch, int newCopies){
    FILE *in=fopen(filename, "r");
    if(in==NULL){
        printf("Error: could not open file.\n");
        return -1;
    }
    FILE *out=fopen(TEMP_NAME, "w");
    if(out==NULL){
        printf("Error: could not open file.\n");
        fclose(in);
        return -1;
    }
    char line[500];
    int found=0;
    while(fgets(line, sizeof line, in)!=NULL){
        Book temp;
        int r = sscanf(line, "%20[^|]|%29[^|]|%100[^|]|%50[^|]|%d|%d|%f",
                temp.ISBN, temp.branch, temp.title, temp.author,
                &temp.copy, &temp.year, &temp.price);
        if(r==7 && strcmp(temp.ISBN, isbn)==0 && strcmp(temp.branch, branch)==0){
            fprintf(out,"%s|%s|%s|%s|%d|%d|%.2f\n",temp.ISBN, temp.branch, temp.title, temp.author, newCopies, temp.year, temp.price);
            found=1;
        }
        else {
            fputs(line, out);                    // copy the line unchanged
        }
    }
    fclose(in);                                  // close BEFORE remove and rename
    fclose(out);
    if(!found){
        remove(TEMP_NAME);
        printf("Book not found.\n");
        return 0;
    }
    remove(filename);
    if(rename(TEMP_NAME, filename)!=0){
        printf("Error: could not rename file.\n");
        return -1;
    }
    return 1;
}

// returns 1 deleted, 0 not found, -1 file problem
int deleteBookInFile(const char *filename, const char *isbn, const char *branch){
    FILE *in=fopen(filename, "r");
    if(in==NULL){
        printf("Error: could not open file.\n");
        return -1;
    }
    FILE *out=fopen(TEMP_NAME, "w");
    if(out==NULL){
        printf("Error: could not open file.\n");
        fclose(in);
        return -1;
    }
    char line[500];
    int found=0;
    while(fgets(line, sizeof line, in)!=NULL){
        Book temp;
        int r = sscanf(line, "%20[^|]|%29[^|]|%100[^|]|%50[^|]|%d|%d|%f",
                temp.ISBN, temp.branch, temp.title, temp.author,
                &temp.copy, &temp.year, &temp.price);
        if(r==7 && strcmp(temp.ISBN, isbn)==0 && strcmp(temp.branch, branch)==0){
            found=1;                             // matching line is NOT written
        }
        else {
            fputs(line, out);                    // copy every other line unchanged
        }
    }
    fclose(in);                                  // close BEFORE remove and rename
    fclose(out);
    if(!found){
        remove(TEMP_NAME);
        printf("Book not found.\n");
        return 0;
    }
    remove(filename);
    if(rename(TEMP_NAME, filename)!=0){
        printf("Error: could not rename file.\n");
        return -1;
    }
    return 1;
}

// ---------- Question 9: sorting and statistics ----------
void sortByPrice(Book library[], int currentSize){      // highest first
    for(int i=0; i<currentSize-1; i++){
        for(int j=0; j<currentSize-1-i; j++){
            if(library[j].price < library[j+1].price){
                Book temp=library[j];                   // swap the whole struct
                library[j]=library[j+1];
                library[j+1]=temp;
            }
        }
    }
}
void sortByTitle(Book library[], int currentSize){      // A to Z
    for(int i=0; i<currentSize-1; i++){
        for(int j=0; j<currentSize-1-i; j++){
            if(strcmp(library[j].title, library[j+1].title) > 0){
                Book temp=library[j];
                library[j]=library[j+1];
                library[j+1]=temp;
            }
        }
    }
}
int totalCopies(const Book library[], int currentSize){
    int total=0;
    for(int i=0; i<currentSize; i++){
        total+=library[i].copy;
    }
    return total;
}
double averagePrice(const Book library[], int currentSize){
    if(currentSize==0) return 0;                        // no divide by zero
    double sum=0;
    for(int i=0; i<currentSize; i++){
        sum+=library[i].price;
    }
    return sum/currentSize;
}
const Book *findMostExpensive(const Book library[], int currentSize){
    if(currentSize==0) return NULL;
    const Book *best=&library[0];
    for(int i=1; i<currentSize; i++){
        if(library[i].price > best->price){
            best=&library[i];
        }
    }
    return best;
}
int countByBranch(const Book library[], int currentSize, const char *branch){
    int count=0;
    for(int i=0; i<currentSize; i++){
        if(strcmp(library[i].branch, branch)==0){
            count++;
        }
    }
    return count;
}

// after update or delete: offer to save
void askToSave(const Book library[], int currentSize){
    char ans;
    printf("Save changes to file now? (y/n)\n");
    scanf(" %c", &ans);
    if(ans=='y' || ans=='Y'){
        saveLibraryToFile(library, currentSize, FILE_NAME);
    }
}

int main(){
    Book library[MAX];
    int currentSize =0;
    int c;
    do{
        printf("1. Add New Book\n");
        printf("2. Display All Books\n");
        printf("0. Exit\n");
        printf("3.search\n");
        printf("4.Restock List\n");
        printf("5.Old Book List\n");
        printf("6.save to file\n");
        printf("7.Load Library from File\n");
        printf("8.Update Copies\n");
        printf("9.Delete Book\n");
        printf("10.Update Copies in File\n");
        printf("11.Sort by Price\n");
        printf("12.Sort by Title\n");
        printf("13.Show Statistics\n");
        printf("14.Delete Book in File\n");
        c=readInt();
        switch(c){
            case 1: {
                if(currentSize>=MAX){
                    printf("the library is full maximum size is 100\n");
                    break;
                }

                Book temp;
                promptBookDetails(&temp);
                if(isDuplicate(library,
                    currentSize, temp.ISBN, temp.branch)!=0){
                    printf("Error: this book already exists in that branch.\n");
                    break;
                }
                library[currentSize]=temp;
                currentSize++;
                printf("book added\n");
                break;

            }
            case 2: {
                if(currentSize==0){
                    printf("library is empty\n");
                    break;
                }
                else {
                    printf("all %d books \n", currentSize);
                    for(int i=0; i<currentSize; i++){
                        displayBook(&library[i]);
                    }
                    break;
                }}
            case 0: printf("goodbye\n");break;
            case 3:{
                    if (currentSize==0){
                        printf("no books in the library\n");
                        break;
                    }
                    printf("enter auithors name\n");
                    char author[51];
                    scanf(" %50[^\n]", author);
                    //no need to take sting array just write array pointer

                    printf("enter max results\n");
                    int maxResults=readInt();
                    if(maxResults<1 || maxResults>MAX){
                        printf("max results must be between 1 and %d\n", MAX);
                        break;
                    }
                    Book *results[MAX];
                    int value = findBooksByAuthor(library, currentSize, author, results, maxResults);
                    if(!value) {
                        printf("no books found for this author\n");
                        break;
                    }
                    printf("found %d books for this author\n", value);
                    for(int i=0; i<value; i++){
                        printf("BOOK:\n");
                        displayBook(results[i]);
                    }
            break;
            }
            case 4: {
                int minc;
                printf("Enter minimum copies: ");
                minc=readInt();
                if(minc<0){
                    printf("limit must be a number 0 or more\n");
                    break;
                }
                int value = getRestockList(library, currentSize, minc);
                printf("Total: %d book(s)\n", value);
                break;
            }
            case 5: {
                int minAge, currentYear;
                printf("Enter current year: ");
                scanf("%d", &currentYear);
                printf("Enter minimum age: ");
                scanf("%d", &minAge);
                int value = getOldBooksList(library, currentSize, currentYear, minAge);
                printf("Total: %d book(s)\n", value);            // matches the question
                break;
            }
            case 6: {int value;
                value=saveLibraryToFile(library, currentSize, FILE_NAME);
                if (value==-1)printf("couldnot create file\n");
                break;
            }
            case 7: {
                int value=loadLibraryFromFile(library, MAX, FILE_NAME);
                if(value!=-1){
                    currentSize=value;               // -1 keeps the old data safe
                }
                break;
            }
            case 8: {
                char isbn[21], branch[30];
                printf("enter isbn\n");
                scanf("%20s", isbn);
                printf("enter the branch\n");
                scanf("%29s", branch);
                printf("enter new copies\n");
                int newCopies=readInt();
                if(newCopies<0){
                    printf("copies must be 0 or more\n");
                    break;
                }
                if(updateCopies(library, currentSize, isbn, branch, newCopies)){
                    printf("copies updated\n");
                    askToSave(library, currentSize);
                }
                else printf("Book not found.\n");
                break;
            }
            case 9: {
                char isbn[21], branch[30];
                printf("enter isbn\n");
                scanf("%20s", isbn);
                printf("enter the branch\n");
                scanf("%29s", branch);
                if(deleteBook(library, &currentSize, isbn, branch)){
                    printf("book deleted\n");
                    askToSave(library, currentSize);
                }
                else printf("Book not found.\n");
                break;
            }
            case 10: {
                char isbn[21], branch[30];
                printf("enter isbn\n");
                scanf("%20s", isbn);
                printf("enter the branch\n");
                scanf("%29s", branch);
                printf("enter new copies\n");
                int newCopies=readInt();
                if(newCopies<0){
                    printf("copies must be 0 or more\n");
                    break;
                }
                if(updateCopiesInFile(FILE_NAME, isbn, branch, newCopies)==1){
                    printf("file updated (use option 7 to load it)\n");
                }
                break;
            }
            case 11: {
                if(currentSize==0){
                    printf("No books in the library.\n");
                    break;
                }
                sortByPrice(library, currentSize);
                for(int i=0; i<currentSize; i++){
                    displayBook(&library[i]);
                }
                break;
            }
            case 12: {
                if(currentSize==0){
                    printf("No books in the library.\n");
                    break;
                }
                sortByTitle(library, currentSize);
                for(int i=0; i<currentSize; i++){
                    displayBook(&library[i]);
                }
                break;
            }
            case 13: {
                if(currentSize==0){
                    printf("No books in the library.\n");
                    break;
                }
                printf("total copies: %d\n", totalCopies(library, currentSize));
                printf("average price: %.2f\n", averagePrice(library, currentSize));
                const Book *best=findMostExpensive(library, currentSize);
                if(best!=NULL){
                    printf("most expensive book:\n");
                    displayBook(best);
                }
                char branch[30];
                printf("enter the branch to count\n");
                scanf("%29s", branch);
                printf("books in %s: %d\n", branch, countByBranch(library, currentSize, branch));
                break;
            }
            case 14: {
                char isbn[21], branch[30];
                printf("enter isbn\n");
                scanf("%20s", isbn);
                printf("enter the branch\n");
                scanf("%29s", branch);
                if(deleteBookInFile(FILE_NAME, isbn, branch)==1){
                    printf("book deleted from file (use option 7 to load it)\n");
                }
                break;
            }
            default : printf("invalid code\n"); break;            }
        }
        while (c!=0);
    return 0;
    }
