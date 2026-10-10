#include <stdio.h>
#include <string.h>

#define MAX 120

typedef struct {
    int roll;
    char name[41];
    char code[10];
    float mark;
    int semester;
} Result;

void displayResult(const Result *resPtr)
{
    printf("Roll is %d\n", resPtr->roll);
    printf("Name is %s\n", resPtr->name);
    printf("Code is %s\n", resPtr->code);
    printf("Mark is %.2f\n", resPtr->mark);
    printf("Semester is %d\n", resPtr->semester);
}

void promptResultDetails(Result *resPtr)
{
    printf("Enter roll: ");
    scanf("%d", &resPtr->roll);

    printf("Enter name: ");
    scanf(" %40[^\n]", resPtr->name);

    // Replace commas with spaces because commas are used as file separators.
    for (int i = 0; resPtr->name[i] != '\0'; i++) {
        if (resPtr->name[i] == ',') {
            resPtr->name[i] = ' ';
        }
    }

    printf("Enter code: ");
    scanf("%9s", resPtr->code);

    do {
        printf("Enter mark: ");
        scanf("%f", &resPtr->mark);

        if (resPtr->mark < 0 || resPtr->mark > 100) {
            printf("Invalid mark. Enter 0 to 100.\n");
        }
    } while (resPtr->mark < 0 || resPtr->mark > 100);

    do {
        printf("Enter semester: ");
        scanf("%d", &resPtr->semester);

        if (resPtr->semester < 1 || resPtr->semester > 8) {
            printf("Invalid semester. Enter 1 to 8.\n");
        }
    } while (resPtr->semester < 1 || resPtr->semester > 8);
}

int duplicate(const Result registry[], const Result *resPtr, int currentSize)
{
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].roll == resPtr->roll &&
            strcmp(registry[i].code, resPtr->code) == 0) {
            return 1;
        }
    }

    return 0;
}

int findResultByRoll(const Result registry[], int currentSize,
                     int searchRoll, Result *results[], int maxResults)
{
    int count = 0;

    for (int i = 0; i < currentSize && count < maxResults; i++) {
        if (registry[i].roll == searchRoll) {
            results[count] = &registry[i];
            count++;
        }
    }

    return count;
}

int getFailedList(const Result registry[], int currentSize, float passMark)
{
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (registry[i].mark < passMark) {
            displayResult(&registry[i]);
            count++;
        }
    }

    return count;
}

int findTopper(const Result registry[], int currentSize,
               const char *courseCode)
{
    int best = -1;

    for (int i = 0; i < currentSize; i++) {
        if (strcmp(registry[i].code, courseCode) == 0) {

            if (best == -1 ||
                registry[i].mark > registry[best].mark) {
                best = i;
            }
        }
    }

    return best;
}
int findTopper(const Result registry[], int currentSize,
    const char *courseCode)
{
    int index = 0;
    int max=0;
    for (int i = 0; i < currentSize; i++) {
        if (strcmp(registry[i].code, courseCode) == 0 && registry[i].mark > registry[best].mark) {
            max=registry[i].mark;
            index=i;
        
    }
}

return index;
}

int saveResultsToFile(const Result registry[], int currentSize,
                      const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Cannot open file\n");
        return -1;
    }

    for (int i = 0; i < currentSize; i++) {
        fprintf(file, "%d,%s,%s,%.2f,%d\n",
                registry[i].roll,
                registry[i].name,
                registry[i].code,
                registry[i].mark,
                registry[i].semester);
    }

    fclose(file);

    return currentSize;
}

int loadResultsFromFile(Result registry[], int maxSize,
                        const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Cannot open file\n");
        return -1;
    }

    char line[400];
    int count = 0;
    int n;

    while (count < maxSize &&
           fgets(line, sizeof(line), file) != NULL) {

        n = sscanf(line, "%d,%40[^,],%9[^,],%f,%d",
                   &registry[count].roll,
                   registry[count].name,
                   registry[count].code,
                   &registry[count].mark,
                   &registry[count].semester);

        if (n == 5) {
            count++;
        }
        else {
            printf("Warning: bad line skipped\n");
        }
    }

    fclose(file);021

    return count;
}

int main(void)
{
    Result student[MAX];
    int currentSize = 0;
    int c;

    do {
        printf("\n");
        printf("1. Add new result\n");
        printf("2. Search result by roll\n");
        printf("3. Display all\n");
        printf("4. Failed student list\n");
        printf("5. Course topper\n");
        printf("6. Save to file\n");
        printf("7. Load from file\n");
        printf("0. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &c);

        switch (c) {

            case 1:
            {
                if (currentSize >= MAX) {
                    printf("Registry is full\n");
                    break;
                }

                Result temp;

                promptResultDetails(&temp);

                if (duplicate(student, &temp, currentSize)) {
                    printf("Duplicate found\n");
                    break;
                }

                student[currentSize] = temp;
                currentSize++;

                printf("Result added successfully\n");

                break;
            }

            case 2:
            {
                int roll;

                printf("Enter roll: ");
                scanf("%d", &roll);

                Result *results[MAX];

                int value = findResultByRoll(
                    student,
                    currentSize,
                    roll,
                    results,
                    MAX
                );

                if (value == 0) {
                    printf("No results found\n");
                    break;
                }

                for (int i = 0; i < value; i++) {
                    displayResult(&esults[i]);
                }

                break;
            }

            case 3:
            {
                if (currentSize > 0) {

                    for (int i = 0; i < currentSize; i++) {
                        displayResult(&student[i]);
                    }

                }
                else {
                    printf("Enter student details first\n");
                }

                break;
            }

            case 4:
            {
                float mark;

                printf("Enter pass mark: ");
                scanf("%f", &mark);

                int value = getFailedList(
                    student,
                    currentSize,
                    mark
                );

                if (value == 0) {
                    printf("Everyone passed\n");
                }

                break;
            }

            case 5:
            {
                char Code[10];

                printf("Enter course code: ");
                scanf("%9s", Code);

                int value = findTopper(
                    student,
                    currentSize,
                    Code
                );

                if (value == -1) {
                    printf("No results for this course.\n");
                    break;
                }

                displayResult(&student[value]);

                break;
            }

            case 6:
            {
                int value = saveResultsToFile(
                    student,
                    currentSize,
                    "saved.txt"
                );

                if (value == -1) {
                    printf("Cannot save file\n");
                    break;
                }

                printf("%d records saved successfully\n", value);

                break;
            }

            case 7:
            {
                int value = loadResultsFromFile(
                    student,
                    MAX,
                    "read.txt"
                );

                if (value == -1) {
                    printf("Cannot load file\n");
                    break;
                }

                currentSize = value;

                printf("%d records loaded successfully\n", value);

                break;
            }

            case 0:
            {
                printf("Exiting program...\n");
                break;
            }

            default:
            {
                printf("Invalid choice\n");
                break;
            }
        }

    } while (c != 0);

    return 0;
}
