#include <stdio.h>
#include <string.h>

#define MAX 150

typedef struct {
    int room;
    char name[101];
    char seatLabel[6];
    char roomType[16];
    float monthlyRent;
    int lastPaidYear;
} SeatRecord;

void promptSeatDetails(SeatRecord *seatPtr);
void displaySeat(const SeatRecord *seatPtr);

int findSeatsByRoom(SeatRecord registry[], int currentSize,
                    int searchRoom, SeatRecord *results[], int maxResults);

int getRentDefaulterList(const SeatRecord registry[],
                         int currentSize, int currentYear);

int findSeatIndex(const SeatRecord registry[], int currentSize,
                  int room, const char *seatLabel);

int vacateSeat(SeatRecord registry[], int *currentSize,
               int room, const char *seatLabel);

int saveRegistryToFile(const SeatRecord registry[],
                       int currentSize, const char *filename);

int loadRegistryFromFile(SeatRecord registry[],
                         int maxSize, const char *filename);

int main(void)
{
    SeatRecord registry[MAX];
    int currentSize = 0;
    int choice;

    do {
        printf("\n===== UNIVERSITY HALL SEAT REGISTRY =====\n");
        printf("1. Add New Seat Record\n");
        printf("2. Search Records by Room Number\n");
        printf("3. Display All Records\n");
        printf("4. Generate Rent Defaulter List\n");
        printf("5. Vacate a Seat\n");
        printf("6. Save Registry to File\n");
        printf("7. Load Registry from File\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            return 1;
        }

        switch (choice) {
            case 1:
            {
                if (currentSize >= MAX) {
                    printf("Registry is full!\n");
                    break;
                }

                SeatRecord temp;
                promptSeatDetails(&temp);

                int duplicate = 0;

                for (int i = 0; i < currentSize; i++) {
                    if (registry[i].room == temp.room &&
                        strcmp(registry[i].seatLabel,
                               temp.seatLabel) == 0) {
                        duplicate = 1;
                        break;
                    }
                }

                if (duplicate) {
                    printf("This room and seat already exist.\n");
                } else {
                    registry[currentSize] = temp;
                    currentSize++;
                    printf("Record added successfully.\n");
                }

                break;
            }

            case 2:
            {
                int searchRoom;
                SeatRecord *results[MAX];

                printf("Enter room number: ");
                scanf("%d", &searchRoom);

                int count = findSeatsByRoom(
                    registry, currentSize, searchRoom, results, MAX
                );

                if (count == 0) {
                    printf("No records found.\n");
                } else {
                    printf("Found %d record(s).\n", count);

                    for (int i = 0; i < count; i++) {
                        displaySeat(results[i]);
                    }
                }

                break;
            }

            case 3:
            {
                if (currentSize == 0) {
                    printf("Registry is empty.\n");
                } else {
                    for (int i = 0; i < currentSize; i++) {
                        displaySeat(&registry[i]);
                    }
                }

                break;
            }

            case 4:
            {
                int currentYear;

                printf("Enter current year: ");
                scanf("%d", &currentYear);

                int count = getRentDefaulterList(
                    registry, currentSize, currentYear
                );

                if (count == 0) {
                    printf("No rent defaulters found.\n");
                } else {
                    printf("Total defaulters: %d\n", count);
                }

                break;
            }

            case 5:
            {
                int room;
                char seatLabel[6];

                printf("Enter room number: ");
                scanf("%d", &room);

                printf("Enter seat label: ");
                scanf("%5s", seatLabel);

                int result = vacateSeat(
                    registry, &currentSize, room, seatLabel
                );

                if (result == 1) {
                    printf("Seat vacated successfully.\n");
                } else {
                    printf("Seat record not found.\n");
                }

                break;
            }

            case 6:
            {
                int saved = saveRegistryToFile(
                    registry, currentSize, "seats.txt"
                );

                if (saved == -1) {
                    printf("Could not open file for saving.\n");
                } else {
                    printf("%d record(s) saved.\n", saved);
                }

                break;
            }

            case 7:
            {
                SeatRecord tempRegistry[MAX];

                int loaded = loadRegistryFromFile(
                    tempRegistry, MAX, "seats.txt"
                );

                if (loaded == -1) {
                    printf("Could not open file for loading.\n");
                } else {
                    for (int i = 0; i < loaded; i++) {
                        registry[i] = tempRegistry[i];
                    }

                    currentSize = loaded;
                    printf("%d record(s) loaded.\n", loaded);
                }

                break;
            }

            case 0:
                printf("Exiting program.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}

void promptSeatDetails(SeatRecord *seatPtr)
{
    printf("Enter room number: ");
    scanf("%d", &seatPtr->room);

    while (seatPtr->room <= 0) {
        printf("Room must be positive. Enter again: ");
        scanf("%d", &seatPtr->room);
    }

    printf("Enter resident name: ");
    scanf(" %100[^\n]", seatPtr->name);

    printf("Enter seat label: ");
    scanf("%5s", seatPtr->seatLabel);

    printf("Enter room type: ");
    scanf(" %15[^\n]", seatPtr->roomType);

    /*
       The pipe character is the file separator.
       Replace it with '-' so it cannot break the file format.
    */
    for (int i = 0; seatPtr->name[i] != '\0'; i++) {
        if (seatPtr->name[i] == '|') {
            seatPtr->name[i] = '-';
        }
    }

    for (int i = 0; seatPtr->seatLabel[i] != '\0'; i++) {
        if (seatPtr->seatLabel[i] == '|') {
            seatPtr->seatLabel[i] = '-';
        }
    }

    for (int i = 0; seatPtr->roomType[i] != '\0'; i++) {
        if (seatPtr->roomType[i] == '|') {
            seatPtr->roomType[i] = '-';
        }
    }

    printf("Enter monthly rent: ");
    scanf("%f", &seatPtr->monthlyRent);

    while (seatPtr->monthlyRent <= 0) {
        printf("Rent must be greater than 0. Enter again: ");
        scanf("%f", &seatPtr->monthlyRent);
    }

    printf("Enter last payment year: ");
    scanf("%d", &seatPtr->lastPaidYear);

    while (seatPtr->lastPaidYear < 1990 ||
           seatPtr->lastPaidYear > 2100) {
        printf("Year must be between 1990 and 2100. Enter again: ");
        scanf("%d", &seatPtr->lastPaidYear);
    }
}

void displaySeat(const SeatRecord *seatPtr)
{
    printf("\n-----------------------------\n");
    printf("Room Number: %d\n", seatPtr->room);
    printf("Resident Name: %s\n", seatPtr->name);
    printf("Seat Label: %s\n", seatPtr->seatLabel);
    printf("Room Type: %s\n", seatPtr->roomType);
    printf("Monthly Rent: %.2f Taka\n", seatPtr->monthlyRent);
    printf("Last Payment Year: %d\n", seatPtr->lastPaidYear);
    printf("-----------------------------\n");
}

int findSeatsByRoom(SeatRecord registry[], int currentSize,
                    int searchRoom, SeatRecord *results[], int maxResults)
{
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (registry[i].room == searchRoom) {
            if (count < maxResults) {
                results[count] = &registry[i];
                count++;
            }
        }
    }

    return count;
}

int getRentDefaulterList(const SeatRecord registry[],
                         int currentSize, int currentYear)
{
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (currentYear - registry[i].lastPaidYear >= 2) {
            displaySeat(&registry[i]);
            count++;
        }
    }

    return count;
}

int findSeatIndex(const SeatRecord registry[], int currentSize,
                  int room, const char *seatLabel)
{
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].room == room &&
            strcmp(registry[i].seatLabel, seatLabel) == 0) {
            return i;
        }
    }

    return -1;
}

int vacateSeat(SeatRecord registry[], int *currentSize,
               int room, const char *seatLabel)
{
    int index = findSeatIndex(
        registry, *currentSize, room, seatLabel
    );

    if (index == -1) {
        return 0;
    }

    for (int i = index; i < *currentSize - 1; i++) {
        registry[i] = registry[i + 1];
    }

    (*currentSize)--;

    return 1;
}

int saveRegistryToFile(const SeatRecord registry[],
                       int currentSize, const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        return -1;
    }

    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        int result = fprintf(
            file, "%d|%s|%s|%s|%.2f|%d\n",
            registry[i].room,
            registry[i].name,
            registry[i].seatLabel,
            registry[i].roomType,
            registry[i].monthlyRent,
            registry[i].lastPaidYear
        );

        if (result < 0) {
            fclose(file);
            return -1;
        }

        count++;
    }

    fclose(file);

    return count;
}

int loadRegistryFromFile(SeatRecord registry[],
                         int maxSize, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        return -1;
    }

    if (maxSize > MAX) {
        maxSize = MAX;
    }

    SeatRecord tempRegistry[MAX];
    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (count >= maxSize) {
            printf("Warning: registry is full; remaining lines skipped.\n");
            break;
        }

        SeatRecord temp;

        int fields = sscanf(
            line, "%d|%100[^|]|%5[^|]|%15[^|]|%f|%d",
            &temp.room,
            temp.name,
            temp.seatLabel,
            temp.roomType,
            &temp.monthlyRent,
            &temp.lastPaidYear
        );

        if (fields == 6 &&
            temp.room > 0 &&
            temp.monthlyRent > 0 &&
            temp.lastPaidYear >= 1990 &&
            temp.lastPaidYear <= 2100) {

            tempRegistry[count] = temp;
            count++;
        } else {
            printf("Warning: damaged or invalid line skipped: %s", line);
        }
    }

    fclose(file);

    for (int i = 0; i < count; i++) {
        registry[i] = tempRegistry[i];
    }

    return count;
}
