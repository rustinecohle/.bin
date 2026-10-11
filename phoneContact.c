#include<stdio.h>
#include<string.h>
#include<ctype.h>

#define MAX 200

typedef struct {
	char name[41];
	char no[12];
	char email[41];
	char group[13];
	int year;
	} Contact ;
int isValidPhone(const char *s){
	

	if(s[0]=='0' && s[1]=='1') return 1;
	else{ printf("invalid mustr start with 01 and atleast 11 characjhters\n"); 
	return 0;
	}

}
int isValidEmail(const char *s){
	int count=0;
	int flag=0;
	for(int i=0; s[i]!='\0'; i++){
		if(s[i]=='@'){
		count++;}
		if(s[i]==' ') flag=1; 
	}
	if(count == 1 && flag==0) return 1;
	else {printf("invalid must have one @ and no spcaes\n");
	return 0;}
}

int duplicate(const Contact *phnPtr, int size,const Contact registrey[]){
	for(int i=0; i<size; i++){
		if(strcmp(registrey[i].no,phnPtr->no)==0)
			return 1;
	
	}
return 0;

}
void addNew(Contact *phnPtr){

	printf("enter name\n");
	scanf(" %40[^\n]", phnPtr->name);
	printf("enter number\n");
	
	do{	scanf(" %11[^\n]", phnPtr->no);
		
	}while(!isValidPhone(phnPtr->no)) ;
	printf("enter email\n");
	
	do{	scanf(" %40[^\n]", phnPtr->email);
		
	}while(!isValidEmail(phnPtr->email));
	printf("enter group\n");
	scanf(" %40[^\n]", phnPtr->group);
	printf("enter birth year\n");
	do{scanf("%d", &phnPtr->year);
	if (phnPtr->year>=2100 || phnPtr->year<=1900){
		printf("enter between 1900 to 2100\n");	
		}
	}while(phnPtr->year>=2100 || phnPtr->year<=1900);

}
void displayContact(const Contact *c){
	printf("\n=========================\n");
	printf("name %s\n", c->name);
	printf("number %s\n", c->no);
	printf("email %s\n\n", c->email);
	printf("group %s\n", c->group);
	printf("year %d\n", c->year);
	printf("\n=========================\n");

}
int seachBYName( Contact registry[], int size, Contact *result[], const char *name, int max){
	int count=0;
	for(int i=0; i<size && count<max; i++){
		if(strcmp(registry[i].name, name)==0){
		result[count]=&registry[i];
		count++;
		}
	}
	return count;

}
int listByGroup(const Contact registry[], int currentSize, const char *group){
	int count=0;
	for(int i=0; i< currentSize ; i++){
		if(strcmp(registry[i].group, group)==0){
			displayContact(&registry[i]);
			count++;
		}
	}
	return count;


}
int saveContactsToFile(Contact registry[], int currentSize,const char *filename){
	FILE *file=fopen(filename, "w");
	if(file==NULL){
		printf("cannot open file\n");
		return -1;
	}
	int count=0;
	for(int i=0; i<currentSize; i++){
		int n=fprintf(file,"%s|%s|%s|%s|%d\n", registry[i].name, registry[i].no,
		 registry[i].email, registry[i].group, registry[i].year);
		if(n>0){
		 count++;
		 }
	
	}
	fclose(file);
	return count;
	

}
int loadContactsFromFile( Contact registry[],int maxSize,const char *filename){
	FILE *file=fopen(filename, "r");
	if(file==NULL){
		printf("cannot open file\n");
		return -1;
	}
	char line[1000];
	int lineno=0;
	int count=0;
	while(fgets(line, sizeof(line), file)!=NULL && count<maxSize ){
		lineno++;
		int n=sscanf(line, " %40[^|]|%11[^|]|%12[^|]|%40[^|]|%d", 				registry[count].name,registry[count].no,registry[count].email,registry[count].group, 	&registry[count].year);
		if(n==5){
		count++;
		
		}
		else printf("warning skipping bad lines\n");
		
		
	}
	fclose(file);
	return count;



}
void sortByName(Contact registry[], int currentSize);

int compareIgnoreCase(const char *a, const char *b);

int containsIgnoreCase(const char *text, const char *keyword);

int main(void){
	int size=0;
	Contact phone[MAX];
	int c;
	do{	printf("\n================PHONE CONTACT================\n");
		printf("1. add contact\n");
		printf("2. Search name \n");
		printf("3. list by group\n");
		printf("4. display all\n");
		printf("5. save to file \n");
		printf("6. load from file\n");
		printf("0. exit\n");
		printf("enter choice \n");
		scanf("%d", &c);
		switch(c){

			case 1:{
				if(size>=MAX){
					printf("max size reached\n");
					
					break;
				}
				
				Contact temp;
				addNew(&temp);
				if(duplicate (&temp, size, phone)){
					printf("dupicate found \n");
					break;
				}
				phone[size]=temp;
				size++;
				break;
			
			
			}
			case 2: {
			Contact *result[MAX];
			char name[41];
			scanf(" %40[^\n]", name);
			int value=seachBYName(phone, size, result, name , MAX);
			if(value>0 ){
				for(int i=0; i<value; i++){
					displayContact(result[i]);
				}
				break;
			}
			printf("no results found\n");
			break;
			}
			case 3:{
				char group[13];
				scanf(" %40[^\n]", group);
				int value=listByGroup(phone, size, group);
				value>0 ? printf("found %d\n", value) : printf("no results found\n");
				break;
			
			}
			
			
			case 4:{
			
				for(int i=0; i<size; i++){
					displayContact(&phone[i]);	
				}
				break;
			
			} 
			case 5: {
			
				int value=saveContactsToFile(phone, size, "saved.txt");
				value> -1 ? printf("wrote %d lines\n", value) : printf("failed\n"); 
				break;
			
			}
			case 6: {
				int value=loadContactsFromFile(phone, MAX , "givenfile.txt");	
			
			
			if(value>-1){
				printf("loaded %d lines", value);
				size=value;
			}
			else printf("cannot open file\n");
			break;
			}
			
			case 0: printf("good bye!\n"); break;
			default:printf("invalid choice\n"); 

		}
	
	
	}while (c!=0);



return 0;
}
// here i skipped the sort by name


