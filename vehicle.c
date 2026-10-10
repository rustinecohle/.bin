/*
 * Mock Final 2: Vehicle Registry
 * Compile: gcc -Wall -Wextra -g vehicle_registry.c -o vehicles
 */
 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include <limits.h>
 
 #define MAX 120
//  #define PLATE_LEN 30
//  #define OWNER_LEN 60
//  #define TYPE_LEN  15
//  #define DATA_FILE "vehicles.txt"
 
 typedef struct {
     char plate[31];
     char owner[61];
     char type[16];
     int  regYear;
     int  cc;
     int  taxYear;
 } Vehicle ;
 


 /* ------------------------------------------------------------------ */
 /* Part 1: prompt and display                                          */
 /* ------------------------------------------------------------------ */
//  void promptVehicleDetails(Vehicle *vehPtr) {
//      readText("Plate number: ", vehPtr->plate, PLATE_LEN, 0);
//      readText("Owner name: ", vehPtr->owner, OWNER_LEN, 0);
//      readText("Vehicle type: ", vehPtr->type, TYPE_LEN, 0);
//      vehPtr->regYear = readIntRange("Registration year (1950-2100): ", 1950, 2100);
//      vehPtr->cc = readIntRange("Engine capacity in cc (greater than 0): ", 1, INT_MAX);
//      while (1) {
//          vehPtr->taxYear = readIntRange("Last tax token year: ", 0, 2100);
//          if (vehPtr->taxYear >= vehPtr->regYear) {
//              break;
//          }
//          printf("  A token cannot be renewed before the vehicle was registered (%d). Try again.\n",
//                 vehPtr->regYear);
//      }
//  }
void promptVehicleDetails(Vehicle *vehPtr) {
    printf("enter plate number\n");
    scanf(" %30[^\n]", vehPtr->plate);
    printf("enter owner name\n");
    scanf(" %60[^\n]", vehPtr->owner);
    printf("enter vehicle type\n");
    scanf(" %15[^\n]", vehPtr->type);

    do {
        printf("enter registration year (1950 to 2100)\n");
        scanf("%d", &vehPtr->regYear);
    } while (vehPtr->regYear < 1950 || vehPtr->regYear > 2100);

    do {
        printf("enter engine capacity in cc (greater than 0)\n");
        scanf("%d", &vehPtr->cc);
    } while (vehPtr->cc <= 0);

    do {
        printf("enter last tax token year (not before %d)\n", vehPtr->regYear);
        scanf("%d", &vehPtr->taxYear);
    } while (vehPtr->taxYear < vehPtr->regYear);
}
 void displayVehicle(const Vehicle *vehPtr) {
     printf("----------------------------------------\n");
     printf("Plate        : %s\n", vehPtr->plate);
     printf("Owner        : %s\n", vehPtr->owner);
     printf("Type         : %s\n", vehPtr->type);
     printf("Registered   : %d\n", vehPtr->regYear);
     printf("Engine       : %d cc\n", vehPtr->cc);
     printf("Last tax year: %d\n", vehPtr->taxYear);
 }
 int duplicate(const Vehicle *vehPtr, Vehicle registry[], int currentSize){
    for (int i=0; i< currentSize; i++){
        if(strcmp(registry[i].plate, vehPtr->plate)==0){
            return 1;
        }
    }
    return 0;
 }
 int findVehiclesByOwner(Vehicle registry[], int currentSize, const char *owner,
    Vehicle *results[], int maxResults){
        int count =0;
        for(int i=0; i<currentSize && count<=maxResults; i++){
            if(strcmp(registry[i].owner, owner)==0){
                results[count]=&registry[i];
                count++;
            }
        }
        return count;
    }   
    int getTaxExpiredList(const Vehicle registry[], int currentSize, int currentYear){
        int count=0;
        for(int i=0; i<currentSize;i++){
            if(registry[i].taxYear<currentYear){
                
                displayVehicle(&registry[i]);
                printf("years overdue:%d\n", currentYear-registry[i].taxYear);
            }
        }
        return count;
    }
    int transferOwnership(Vehicle registry[], int currentSize, const char *plate,
        const char *newOwner){
            for(int i=0; i<currentSize; i++){
                if(strcmp(registry[i].plate, plate)==0){
                    strcpy(registry[i].owner,newOwner);
                    return 1;
                }
            }
            return 0;
        }
        int searchByPlate(const Vehicle registry[], int currentSize, const char *plate){
            for(int i=0; i<currentSize;i++){
                if(strcmp(registry[i].plate, plate)==0){
                    return i;
                }
            }
            return -1;
        }

        int deregisterVehicle(Vehicle registry[], int *currentSize, const char *plate){
            int value=searchByPlate(registry, *currentSize, plate);
            if (value==-1){
                return 0;
            }
            for(int i=value; i<*currentSize-1; i++){
                registry[i]=registry[i+1];
            }
            (*currentSize)--;
            return 1;
        }
        int saveRegistryToFile(const Vehicle registry[], char *filename, int currentSize){
            FILE *file=fopen(filename, "w");
            if(file==NULL){
                return -1;
            }
            int count=0;
            for(int i=0; i<currentSize; i++){
                int n=fprintf(file, "%s|%s|%s|%d|%d|%d\n", registry[count].plate,
                registry[count].owner, registry[count].type, registry[count].regYear,
                registry[count].cc, registry[count].taxYear);
                    if (n==6) count++;
            }
            
            fclose(file);
            return count;
        }
        int loadRegistryFromFile(const Vehicle registry[], 
            int maxSize, char *filename){
                FILE *file=fopen(filename, "r");
                if (file==NULL){
                    return -1;
                }
                char line[400];
                int count=0;
                int lineno=0;
                while(fgets(line, sizeof(line), file)!=NULL && count <= maxSize){
                    lineno++;
                    int n=sscanf(line, "%30[^|]|%60[^|]|%15[^|]|%d|%d|%d|", registry[count].plate,
                    registry[count].owner, registry[count].type, &registry[count].regYear, 
                    &registry[count].cc, &registry[count].taxYear );
                    if(n==6){
                        count++;
                    }
                    else {
                        printf("warning skipping bad lines\n");
                    }

                }
                return count;
            }

 /* ------------------------------------------------------------------ */
 int main(void) {
     Vehicle registry[MAX];
     int currentSize = 0;
     int choice;
 
     do {
         printf("\n1. Register New Vehicle\n");
         printf("2. Search by Plate Number\n");
         printf("3. Show Vehicles of an Owner\n");
         printf("4. Display All Records\n");
         printf("5. Tax Token Expired List\n");
         printf("6. Transfer Ownership\n");
         printf("7. Deregister a Vehicle\n");
         printf("8. Save Registry to File\n");
         printf("9. Load Registry from File\n");
         printf("0. Exit\n");
         scanf("%d", &choice);
 
         switch (choice) {
             
            case 1: {
                if(currentSize>=MAX){
                    printf("max size reached quitting\n");
                    break;
                }
                Vehicle temp;
                promptVehicleDetails(&temp);
                int value=duplicate( &temp, registry, currentSize);
                if(value==0){
                    printf("duplicate found\n");
                }
                else {
                    registry[currentSize]=temp;
                    currentSize++;
                }
                break;
            }
            case 2: {
                Vehicle *results[MAX];
                char name[61];
                scanf(" %[^\n]", name);
                int value=findVehiclesByOwner(registry, currentSize, name, results, MAX);
                if(value==0){
                    printf("no results found\n");
                }
                else{ printf("found %d results\n", value);
                    for(int i=0; i<currentSize; i++){
                        displayVehicle(results[i]);
                    }
                }
                break;
            }
            case 3: {
                if(currentSize==0){
                    printf("no data here\n");
                    break;
                }
                for (int i=0; i<currentSize; i++){
                    displayVehicle(&registry[i]);
                }
                break;
            }
            case 4: {
                if(currentSize==0){
                    printf("enter data first\n");
                    break;
                }
                for(int i=0; i<currentSize; i++){
                    displayVehicle(&registry[i]);
                }
                break;
                
            }
            case 5:
            {
                int year;
                printf("enter current year\n");
                scanf("%d", &year);
                int value=getTaxExpiredList(registry, currentSize, year);
                if(value==0){
                    printf("no results found \n");
                }
                else {
                    printf("%d results found\n", value);
                }
                break;
            }
            case 6:{
                char plate[31];
                printf("enter plate number to search \n");
                scanf(" %30[^\n]", plate);
                char newOwner[61];
                printf("enter mew owner name\n");
                scanf(" %60[^\n]", newOwner);
                int value=transferOwnership(registry, currentSize, plate, newOwner);
                value ==1 ? printf("found and changed\n") : printf("failed\n");
                break;  
            }
            case 7: {
                char plate[31];
                printf("enter plate number\n");
                scanf(" %30[^\n]", plate);
                int value=deregisterVehicle(registry, &currentSize, plate);
                value ==1 ? printf("deregistration done\n") : printf("failed\n");
                break;
            }
            case 8:{
                int value = saveRegistryToFile(registry, "svehicle.txt", currentSize);
                if (value==0){
                    printf("cannot open file\n");

                }
                else {
                    printf("wrote %d lines succesfully\n", value);
                }
                break;
            }
            case 9: {
                int value=loadRegistryFromFile(registry, MAX, "getvehicle.txt");
                if (value==-1){
                    printf("failed opening file\n");
                }
                else printf("wrote %d line\n", value);
                break;
            }
            case 0:
                 printf("Goodbye!\n");
                 break;
            default : printf("invalid keyword\n");
         }
     } while (choice != 0);
 
     return 0;
 }
