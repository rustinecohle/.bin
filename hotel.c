```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>

#define MAX 120
#define FILE_NAME "hotel.txt"

typedef struct {
    int bookingId;
    int room;
    char guestName[41];
    int checkInDay;
    int nights;
    float rate;
} Booking;

/* ---------- INPUT FUNCTIONS ---------- */

void readLine(char *s, int size) {
    if (fgets(s, size, stdin) == NULL) {
        exit(0);
    }

    s[strcspn(s, "\n")] = '\0';

    /* Remove CR if the line uses Windows line endings. */
    s[strcspn(s, "\r")] = '\0';
}

int readInt(void) {
    char line[128];
    char *end;
    long value;

    while (1) {
        readLine(line, sizeof(line));
        errno = 0;
        value = strtol(line, &end, 10);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (end != line && *end == '\0' &&
            errno != ERANGE && value >= -2147483647L - 1 &&
            value <= 2147483647L) {
            return (int)value;
        }

        printf("Invalid integer. Try again: ");
    }
}

float readFloat(void) {
    char line[128];
    char *end;
    float value;

    while (1) {
        readLine(line, sizeof(line));
        errno = 0;
        value = strtof(line, &end);

        while (isspace((unsigned char)*end)) {
            end++;
        }

        if (end != line && *end == '\0' &&
            errno != ERANGE && isfinite(value)) {
            return value;
        }

        printf("Invalid number. Try again: ");
    }
}

int isBlank(const char *s) {
    while (*s) {
        if (!isspace((unsigned char)*s)) {
            return 0;
        }
        s++;
    }

    return 1;
}

/* ---------- VALIDATION ---------- */

int isValidBooking(const Booking *bPtr) {
    if (bPtr->bookingId <= 0)
        return 0;

    if (bPtr->room < 1 || bPtr->room > 50)
        return 0;

    if (strlen(bPtr->guestName) == 0 ||
        strlen(bPtr->guestName) > 40 ||
        isBlank(bPtr->guestName))
        return 0;

    if (bPtr->checkInDay < 1 || bPtr->checkInDay > 365)
        return 0;

    if (bPtr->nights < 1 || bPtr->nights > 30)
        return 0;

    if (bPtr->checkInDay + bPtr->nights - 1 > 365)
        return 0;

    if (bPtr->rate <= 0 || !isfinite(bPtr->rate))
        return 0;

    return 1;
}

/* ---------- PROMPT AND DISPLAY ---------- */

void promptBookingDetails(Booking *bPtr) {
    while (1) {
        printf("\nEnter booking ID: ");
        bPtr->bookingId = readInt();

        printf("Enter room (1-50): ");
        bPtr->room = readInt();

        printf("Enter guest name: ");
        readLine(bPtr->guestName, sizeof(bPtr->guestName));

        printf("Enter check-in day (1-365): ");
        bPtr->checkInDay = readInt();

        printf("Enter nights (1-30): ");
        bPtr->nights = readInt();

        printf("Enter rate per night: ");
        bPtr->rate = readFloat();

        if (isValidBooking(bPtr)) {
            return;
        }

        printf("\nInvalid booking details. Try again.\n");
    }
}

void displayBooking(const Booking *bPtr) {
    printf("Booking ID: %d\n", bPtr->bookingId);
    printf("Room: %d\n", bPtr->room);
    printf("Guest: %s\n", bPtr->guestName);
    printf("Check-in day: %d\n", bPtr->checkInDay);

    printf("Occupied days: %d to %d\n",
           bPtr->checkInDay,
           bPtr->checkInDay + bPtr->nights - 1);

    printf("Check-out day: %d\n",
           bPtr->checkInDay + bPtr->nights);

    printf("Nights: %d\n", bPtr->nights);
    printf("Rate per night: %.2f\n", bPtr->rate);

    printf("Total cost: %.2f\n",
           bPtr->nights * bPtr->rate);

    printf("-------------------------\n");
}

/* ---------- OVERLAP ---------- */

int bookingsOverlap(const Booking *a, const Booking *b) {
    int firstA = a->checkInDay;
    int lastA = a->checkInDay + a->nights - 1;

    int firstB = b->checkInDay;
    int lastB = b->checkInDay + b->nights - 1;

    if (a->room != b->room)
        return 0;

    if (firstA <= lastB && firstB <= lastA)
        return 1;

    return 0;
}

/* ---------- DUPLICATE ID ---------- */

int isDuplicateId(const Booking registry[],
                  int currentSize, int bookingId) {
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].bookingId == bookingId) {
            return 1;
        }
    }

    return 0;
}

/* ---------- ADD BOOKING ---------- */

/*
Returns:
  1  = added
  0  = duplicate ID
 -1  = overlap
 -2  = registry full
*/

int addBooking(Booking registry[], int *currentSize,
               const Booking *bPtr) {
    if (*currentSize >= MAX)
        return -2;

    if (isDuplicateId(registry, *currentSize,
                      bPtr->bookingId))
        return 0;

    for (int i = 0; i < *currentSize; i++) {
        if (bookingsOverlap(&registry[i], bPtr))
            return -1;
    }

    registry[*currentSize] = *bPtr;
    (*currentSize)++;

    return 1;
}

/* ---------- CANCEL BOOKING ---------- */

int cancelBooking(Booking registry[], int *currentSize,
                  int bookingId) {
    int index = -1;

    for (int i = 0; i < *currentSize; i++) {
        if (registry[i].bookingId == bookingId) {
            index = i;
            break;
        }
    }

    if (index == -1)
        return 0;

    for (int i = index; i < *currentSize - 1; i++) {
        registry[i] = registry[i + 1];
    }

    (*currentSize)--;

    return 1;
}

/* ---------- SEARCH BY ROOM ---------- */

int findBookingsByRoom(Booking registry[], int currentSize,
                       int room, Booking *results[],
                       int maxResults) {
    int count = 0;

    for (int i = 0; i < currentSize &&
                    count < maxResults; i++) {
        if (registry[i].room == room) {
            results[count] = &registry[i];
            count++;
        }
    }

    return count;
}

/* ---------- WHO IS STAYING ON A DAY ---------- */

int getStayingOnDay(const Booking registry[],
                    int currentSize, int day) {
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        int lastDay = registry[i].checkInDay
                    + registry[i].nights - 1;

        if (registry[i].checkInDay <= day &&
            day <= lastDay) {
            displayBooking(&registry[i]);
            count++;
        }
    }

    if (count == 0) {
        printf("Nobody is staying on day %d.\n", day);
    }

    return count;
}

/* ---------- FIRST FREE SLOT ---------- */

int firstFreeDay(const Booking registry[], int currentSize,
                 int room, int nights) {
    if (nights < 1 || nights > 365)
        return -1;

    for (int day = 1; day + nights - 1 <= 365; day++) {
        Booking temp = {0};

        temp.room = room;
        temp.checkInDay = day;
        temp.nights = nights;

        int available = 1;

        for (int i = 0; i < currentSize; i++) {
            if (bookingsOverlap(&registry[i], &temp)) {
                available = 0;
                break;
            }
        }

        if (available) {
            return day;
        }
    }

    return -1;
}

/* ---------- REVENUE ---------- */

float totalRevenue(const Booking registry[],
                   int currentSize) {
    float total = 0;

    for (int i = 0; i < currentSize; i++) {
        total += registry[i].nights * registry[i].rate;
    }

    return total;
}

/* ---------- ENCODE GUEST NAME ---------- */

/*
The separator is |.
Encode | as %7C and % as %25 before saving.
This preserves names containing the separator and literal
escape sequences when the file is loaded again.
*/

void encodeName(const char *source, char *dest) {
    while (*source) {
        if (*source == '|') {
            *dest++ = '%';
            *dest++ = '7';
            *dest++ = 'C';
        }
        else if (*source == '%') {
            *dest++ = '%';
            *dest++ = '2';
            *dest++ = '5';
        }
        else {
            *dest++ = *source;
        }

        source++;
    }

    *dest = '\0';
}

int decodeName(const char *source, char *dest) {
    int i = 0;

    while (*source) {
        if (*source == '%') {
            if (source[1] == '7' && source[2] == 'C') {
                dest[i++] = '|';
                source += 3;
            }
            else if (source[1] == '2' && source[2] == '5') {
                dest[i++] = '%';
                source += 3;
            }
            else {
                return 0;
            }
        }
        else {
            dest[i++] = *source++;
        }

        if (i > 40)
            return 0;
    }

    dest[i] = '\0';
    return 1;
}

/* ---------- SAVE TO FILE ---------- */

int saveRegistryToFile(const Booking registry[],
                       int currentSize, const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    if (fprintf(fp, "HOTEL1\n") < 0) {
        fclose(fp);
        return -1;
    }

    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        char encodedName[121];

        encodeName(registry[i].guestName, encodedName);

        if (fprintf(fp, "%d|%d|%s|%d|%d|%.9g\n",
                    registry[i].bookingId,
                    registry[i].room,
                    encodedName,
                    registry[i].checkInDay,
                    registry[i].nights,
                    (double)registry[i].rate) < 0) {
            fclose(fp);
            printf("Error writing file.\n");
            return -1;
        }

        count++;
    }

    if (fclose(fp) != 0) {
        printf("Error closing file.\n");
        return -1;
    }

    printf("Saved %d booking(s).\n", count);
    return count;
}

/* ---------- LOAD FROM FILE ---------- */

int loadRegistryFromFile(Booking registry[], int maxSize,
                         const char *filename) {
    if (maxSize < 0 || maxSize > MAX)
        return -1;

    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    char line[256];

    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        printf("Missing signature.\n");
        return -1;
    }

    line[strcspn(line, "\n")] = '\0';
    line[strcspn(line, "\r")] = '\0';

    if (strcmp(line, "HOTEL1") != 0) {
        fclose(fp);
        printf("Wrong file signature.\n");
        return -1;
    }

    Booking tempRegistry[MAX];
    int count = 0;
    int lineNo = 1;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNo++;

        /* Reject a physical line too long for our buffer. */
        if (strchr(line, '\n') == NULL && !feof(fp)) {
            int ch;

            while ((ch = fgetc(fp)) != '\n' && ch != EOF) {
            }

            printf("Warning: invalid line %d.\n", lineNo);
            continue;
        }

        Booking temp;
        char encodedName[121];
        int used = 0;

        int r = sscanf(line,
                       "%d|%d|%120[^|]|%d|%d|%f%n",
                       &temp.bookingId,
                       &temp.room,
                       encodedName,
                       &temp.checkInDay,
                       &temp.nights,
                       &temp.rate,
                       &used);

        /* Parsing must consume the entire record. */
        int complete = (r == 6);

        if (complete) {
            while (line[used] != '\0' &&
                   isspace((unsigned char)line[used])) {
                used++;
            }

            if (line[used] != '\0')
                complete = 0;
        }

        if (!complete ||
            !decodeName(encodedName, temp.guestName) ||
            !isValidBooking(&temp)) {
            printf("Warning: invalid record on line %d.\n",
                   lineNo);
            continue;
        }

        if (isDuplicateId(tempRegistry, count, temp.bookingId)) {
            printf("Warning: duplicate ID on line %d.\n",
                   lineNo);
            continue;
        }

        int overlap = 0;

        for (int i = 0; i < count; i++) {
            if (bookingsOverlap(&tempRegistry[i], &temp)) {
                overlap = 1;
                break;
            }
        }

        if (overlap) {
            printf("Warning: overlapping booking on line %d.\n",
                   lineNo);
            continue;
        }

        if (count >= maxSize) {
            fclose(fp);
            printf("Not enough room to load the file.\n");
            return -1;
        }

        tempRegistry[count] = temp;
        count++;
    }

    if (ferror(fp)) {
        fclose(fp);
        printf("Error reading file.\n");
        return -1;
    }

    if (fclose(fp) != 0) {
        printf("Error closing file.\n");
        return -1;
    }

    /* Copy only after the complete file has been read successfully. */
    for (int i = 0; i < count; i++) {
        registry[i] = tempRegistry[i];
    }

    printf("Loaded %d booking(s).\n", count);
    return count;
}

/* ---------- MAIN MENU ---------- */

int main(void) {
    Booking registry[MAX];
    int currentSize = 0;
    int choice;

    do {
        printf("\n===== HOTEL BOOKING REGISTRY =====\n");
        printf("1. New Booking\n");
        printf("2. Bookings of a Room\n");
        printf("3. Display All\n");
        printf("4. Who Is Staying on a Day\n");
        printf("5. Find First Free Slot\n");
        printf("6. Cancel Booking\n");
        printf("7. Revenue\n");
        printf("8. Save to File\n");
        printf("9. Load from File\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        choice = readInt();

        switch (choice) {
            case 1: {
                if (currentSize >= MAX) {
                    printf("Registry is full.\n");
                    break;
                }

                Booking temp;
                promptBookingDetails(&temp);

                int value = addBooking(
                    registry, &currentSize, &temp
                );

                if (value == 1) {
                    printf("Booking added successfully.\n");
                }
                else if (value == 0) {
                    printf("Error: duplicate booking ID.\n");
                }
                else if (value == -1) {
                    printf("Error: booking overlaps with ID(s): ");

                    for (int i = 0; i < currentSize; i++) {
                        if (bookingsOverlap(&registry[i], &temp)) {
                            printf("%d ", registry[i].bookingId);
                        }
                    }

                    printf("\n");
                }
                else if (value == -2) {
                    printf("Registry is full.\n");
                }

                break;
            }

            case 2: {
                int room;

                printf("Enter room number: ");
                room = readInt();

                Booking *results[MAX];

                int value = findBookingsByRoom(
                    registry, currentSize, room,
                    results, MAX
                );

                if (value == 0) {
                    printf("No bookings found.\n");
                    break;
                }

                printf("Found %d booking(s).\n", value);

                for (int i = 0; i < value; i++) {
                    displayBooking(results[i]);
                }

                break;
            }

            case 3: {
                if (currentSize == 0) {
                    printf("No bookings in registry.\n");
                    break;
                }

                for (int i = 0; i < currentSize; i++) {
                    displayBooking(&registry[i]);
                }

                break;
            }

            case 4: {
                int day;

                printf("Enter day (1-365): ");
                day = readInt();

                if (day < 1 || day > 365) {
                    printf("Invalid day.\n");
                    break;
                }

                int value = getStayingOnDay(
                    registry, currentSize, day
                );

                printf("Total staying: %d\n", value);
                break;
            }

            case 5: {
                int room, nights;

                printf("Enter room number: ");
                room = readInt();

                printf("Enter required nights: ");
                nights = readInt();

                int day = firstFreeDay(
                    registry, currentSize, room, nights
                );

                if (day == -1) {
                    printf("No suitable free slot found.\n");
                }
                else {
                    printf("Earliest check-in day: %d\n", day);
                }

                break;
            }

            case 6: {
                int bookingId;

                printf("Enter booking ID to cancel: ");
                bookingId = readInt();

                if (cancelBooking(
                        registry, &currentSize, bookingId)) {
                    printf("Booking cancelled.\n");
                }
                else {
                    printf("Booking ID not found.\n");
                }

                break;
            }

            case 7: {
                printf("Total revenue: %.2f\n",
                       totalRevenue(registry, currentSize));
                break;
            }

            case 8: {
                int value = saveRegistryToFile(
                    registry, currentSize, FILE_NAME
                );

                if (value == -1) {
                    printf("Could not save registry.\n");
                }

                break;
            }

            case 9: {
                int value = loadRegistryFromFile(
                    registry, MAX, FILE_NAME
                );

                if (value != -1) {
                    currentSize = value;
                }

                break;
            }

            case 0:
                printf("Goodbye.\n");
                break;

            default:
                printf("Invalid choice.\n");
        }

    } while (choice != 0);

    return 0;
}
```
