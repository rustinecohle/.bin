#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 10
typedef struct {
    int id;
    char name[101];
    int no;
    char type[21];
    char date[11];
    int rent;
    int status;

}Reservation;

void addNew( Reservation *roomPtr){
    printf("1.enter id\n");
    scanf("%d", &roomPtr->id);
    printf("2.enter name\n");
    scanf(" %100[^\n]", roomPtr->name);
    printf("3.enter number\n");
    scanf("%d", &roomPtr->no);
    printf("4.enter type\n");
    scanf(" %20[^\n]", roomPtr->type);
    printf("5.enter date\n");
    scanf(" %[^\n]", roomPtr->date);
    printf("6.enter rent\n");
    scanf("%d", &roomPtr->rent);
    roomPtr->status=1;
    return;
}
void display(const Reservation registry[], int index){
    printf("id %d\n", registry[index].id);
    printf("name %s\n", registry[index].name);
    printf("room number %d\n", registry[index].no);
    printf("room type %s\n", registry[index].type);
    printf("date %s\n", registry[index].date);
    printf("rent %d\n", registry[index].rent);
    printf("status %d\n", registry[index].status);
}
int searchByID(const Reservation registry[],int currenSize, int id){

    for(int i=0; i< currenSize; i++){
        if (registry[i].id==id)
        return i;
    }
    return 0;
}
int searchByDate(const Reservation registry[], int currenSize, const char *date)
{
    for(int i = 0; i < currenSize; i++) {
        if(strcmp(registry[i].date, date) == 0)
            return i;
    }

    return -1;
}
// void vacant(const Reservation registry[], int size ){
//     for (int i=0; i< size; i++){
//         if(registry[i].status == 0){
//             printf("room number %d is vacant\n", i+1);
//         }
//     }
// return;
// }
void vacant(const Reservation registry[], int size)
{
    for (int roomNo = 1; roomNo <= MAX; roomNo++) {
        int occupied = 0;

        for (int i = 0; i < size; i++) {
            if (registry[i].no == roomNo && registry[i].status == 1) {
                occupied = 1;
                break;
            }
        }

        if (occupied == 0) {
            printf("room number %d is vacant\n", roomNo);
        }
    }
}
int revenue(const Reservation registry[], int currenSize){
    int total=0;
    for(int i=0; i<currenSize; i++){
        total += registry[i].rent;
    }
    return (total*365);
}
int main(void){
    Reservation room[MAX];
    int currenSize=0;
    int c;
    do{ printf("1.add new reservation\n");
        printf("2.search reservation by ID\n");
        printf("3.Display all reservation by date\n");
        printf("4.Display vacant room\n");
        printf("5.calculate total yearly revenue\n");
        printf("0.exit\n");
       

        scanf("%d", &c);
        switch(c){
            case 1:{ Reservation temp; 
                addNew(&temp);
                room[currenSize]=temp;
                currenSize++;
                printf("room %d added successfully\n", currenSize);
                break;

            }
            case 2:{
                int id;
                printf("enter ID");
                scanf("%d", &id);
                int value=searchByID(room,currenSize, id);
                if(value==0){
                    printf("no rooms found\n");
                    break;
                }
                display(room, value);
                break;
            }
            case 3: {
                char date[11];
            
                printf("enter date\n");
                scanf("%10s", date);
            
                int value = searchByDate(room, currenSize, date);
            
                if(value == -1) {
                    printf("no rooms found\n");
                    break;
                }
            
                display(room, value);
                break;
            }
            case 4:{
                // int value=vacant(room, currenSize);
                // if(value=0){
                //     printf("all rooms are occupied\n");
                //     break;
                // }
                // else display(room, vale); break;
                
                vacant(room, currenSize);
                break;
            
            }
            case 6:
            {
                int value=revenue(room, currenSize);
                printf("yearly revrenue is %d\n", value);
                break;
            }
            case 0:printf("goodbye\n"); break;
            default: printf("invalid choice\n");break;

        }
    }while(c!=0);
    return 0;

}
