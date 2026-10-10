
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 150

typedef struct {
    int code;
    char name[51];
    char batch[16];
    int quantity;
    float price;
    int year;
} Medicine;

/* Function declarations */
void promptMedicineDetails(Medicine *medPtr);
void displayMedicine(const Medicine *medPtr);

int isDuplicate(const Medicine registry[], int currentSize,
                const Medicine *medPtr);

int findMedicinesByCode(Medicine registry[], int currentSize,
                        int searchCode, Medicine *results[],
                        int maxResults);

int getExpiredList(const Medicine registry[], int currentSize,
                   int currentYear);

int saveRegistryToFile(const Medicine registry[], int currentSize,
                       const char *filename);

int loadRegistryFromFile(Medicine registry[], int maxSize,
                         const char *filename);

int searchByCodeBatch(const Medicine registry[], int currentSize,
                      int code, const char *batch);

int sellMedicine(Medicine registry[], int currentSize,
                 int code, const char *batch, int qty);


/* Read one medicine */
void promptMedicineDetails(Medicine *medPtr)
{
    printf("Enter medicine code: ");
    scanf("%d", &medPtr->code);

    printf("Enter medicine name: ");
    scanf(" %50[^\n]", medPtr->name);

    printf("Enter batch number: ");
    scanf(" %15[^\n]", medPtr->batch);

    printf("Enter quantity: ");
    scanf("%d", &medPtr->quantity);

    printf("Enter price: ");
    scanf("%f", &medPtr->price);

    printf("Enter expiry year: ");
    scanf("%d", &medPtr->year);
}


/* Display one medicine */
void displayMedicine(const Medicine *medPtr)
{
    printf("----------------------------\n");
    printf("Code     : %d\n", medPtr->code);
    printf("Name     : %s\n", medPtr->name);
    printf("Batch    : %s\n", medPtr->batch);
    printf("Quantity : %d\n", medPtr->quantity);
    printf("Price    : %.2f\n", medPtr->price);
    printf("Expiry   : %d\n", medPtr->year);
}


/* Check whether a string contains | */
int containsPipe(const char *str)
{
    return strchr(str, '|') != NULL;
}


/* Return 1 if code AND batch already exist */
int isDuplicate(const Medicine registry[], int currentSize,
                const Medicine *medPtr)
{
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].code == medPtr->code &&
            strcmp(registry[i].batch, medPtr->batch) == 0) {
            return 1;
        }
    }

    return 0;
}


/* Find all batches with the given code */
int findMedicinesByCode(Medicine registry[], int currentSize,
                        int searchCode, Medicine *results[],
                        int maxResults)
{
    int count = 0;

    for (int i = 0; i < currentSize && count < maxResults; i++) {
        if (registry[i].code == searchCode) {
            results[count] = &registry[i];
            count++;
        }
    }

    for (int i = 0; i < count; i++) {
        displayMedicine(results[i]);
    }

    return count;
}


/* Display expired medicines and return their count */
int getExpiredList(const Medicine registry[], int currentSize,
                   int currentYear)
{
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (registry[i].year < currentYear) {
            displayMedicine(&registry[i]);
            count++;
        }
    }

    return count;
}


/* Save all records */
int saveRegistryToFile(const Medicine registry[], int currentSize,
                       const char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL) {
        printf("Cannot open file for saving.\n");
        return -1;
    }

    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        int written = fprintf(file, "%d|%s|%s|%d|%.2f|%d\n",
                              registry[i].code,
                              registry[i].name,
                              registry[i].batch,
                              registry[i].quantity,
                              registry[i].price,
                              registry[i].year);

        if (written < 0) {
            fclose(file);
            return -1;
        }

        count++;
    }

    if (fclose(file) != 0) {
        return -1;
    }

    return count;
}


/* Load valid records, skipping damaged lines */
int loadRegistryFromFile(Medicine registry[], int maxSize,
                         const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        printf("Cannot open file for loading.\n");
        return -1;
    }

    if (maxSize < 0) {
        fclose(file);
        return -1;
    }

    Medicine *temp = NULL;

    if (maxSize > 0) {
        temp = malloc((size_t)maxSize * sizeof(Medicine));

        if (temp == NULL) {
            fclose(file);
            return -1;
        }
    }

    char line[400];
    int count = 0;
    int linecount = 0;

    while (fgets(line, sizeof(line), file) != NULL &&
           count < maxSize) {

        linecount++;

        Medicine med;
        char extra;

        int n = sscanf(line, "%d|%50[^|]|%15[^|]|%d|%f|%d %c",
                       &med.code,
                       med.name,
                       med.batch,
                       &med.quantity,
                       &med.price,
                       &med.year,
                       &extra);

        if (n != 6 ||
            med.code <= 0 ||
            med.quantity < 0 ||
            med.price < 0.0f ||
            med.year <= 0 ||
            containsPipe(med.name) ||
            containsPipe(med.batch)) {

            printf("Warning: skipping bad line %d.\n", linecount);
            continue;
        }

        /* Keep the first occurrence of each code-and-batch pair. */
        if (isDuplicate(temp, count, &med)) {
            printf("Warning: duplicate record on line %d.\n",
                   linecount);
            continue;
        }

        temp[count] = med;
        count++;
    }

    if (ferror(file)) {
        printf("Error while reading the file.\n");
        free(temp);
        fclose(file);
        return -1;
    }

    fclose(file);

    /*
       Copy into the real registry only after loading succeeds.
       If opening or reading fails, existing records stay unchanged.
    */
    for (int i = 0; i < count; i++) {
        registry[i] = temp[i];
    }

    free(temp);
    return count;
}


/* Return the index of a medicine, or -1 */
int searchByCodeBatch(const Medicine registry[], int currentSize,
                      int code, const char *batch)
{
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].code == code &&
            strcmp(registry[i].batch, batch) == 0) {
            return i;
        }
    }

    return -1;
}


/* Sell stock only if the complete request is valid */
int sellMedicine(Medicine registry[], int currentSize,
                 int code, const char *batch, int qty)
{
    if (qty <= 0) {
        return 0;
    }

    int index = searchByCodeBatch(registry, currentSize,
                                  code, batch);

    if (index == -1) {
        return 0;
    }

    if (qty > registry[index].quantity) {
        return 0;
    }

    registry[index].quantity -= qty;
    return 1;
}


int main(void)
{
    Medicine record[MAX];
    int currentSize = 0;
    int c;

    do {
        printf("\n===== PHARMACY INVENTORY =====\n");
        printf("1. Add New Medicine\n");
        printf("2. Search by Medicine Code\n");
        printf("3. Display All\n");
        printf("4. Expired Medicines List\n");
        printf("5. Sell Medicine\n");
        printf("6. Save to File\n");
        printf("7. Load from File\n");
        printf("0. Exit\n");
        printf("Choice: ");

        if (scanf("%d", &c) != 1) {
            printf("Invalid input. Enter a menu number.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
                /* Discard invalid input */
            }

            continue;
        }

        switch (c) {
            case 1: {
                if (currentSize >= MAX) {
                    printf("Registry is full.\n");
                    break;
                }

                Medicine temp;
                promptMedicineDetails(&temp);

                if (temp.code <= 0 ||
                    temp.quantity < 0 ||
                    temp.price < 0.0f ||
                    temp.year <= 0) {
                    printf("Invalid medicine details.\n");
                    break;
                }

                /*
                   The | character is forbidden in names and batches
                   because it separates fields in the saved file.
                */
                if (containsPipe(temp.name) ||
                    containsPipe(temp.batch)) {
                    printf("The | character is not allowed.\n");
                    break;
                }

                if (isDuplicate(record, currentSize, &temp)) {
                    printf("Error: this code and batch already exist.\n");
                    break;
                }

                record[currentSize] = temp;
                currentSize++;

                printf("Medicine added. Total: %d\n", currentSize);
                break;
            }

            case 2: {
                int value;
                Medicine *results[MAX];

                printf("Enter medicine code: ");

                if (scanf("%d", &value) != 1) {
                    printf("Invalid code.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {
                    }
                    break;
                }

                int found = findMedicinesByCode(
                    record, currentSize, value, results, MAX
                );

                if (found == 0) {
                    printf("No medicine found with this code.\n");
                } else {
                    printf("Found %d batch(es).\n", found);
                }

                break;
            }

            case 3: {
                if (currentSize == 0) {
                    printf("No medicines right now.\n");
                    break;
                }

                for (int i = 0; i < currentSize; i++) {
                    displayMedicine(&record[i]);
                }

                break;
            }

            case 4: {
                int year;

                printf("Enter current year: ");

                if (scanf("%d", &year) != 1) {
                    printf("Invalid year.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {
                    }
                    break;
                }

                int value = getExpiredList(record, currentSize, year);

                if (value == 0) {
                    printf("No expired medicines.\n");
                } else {
                    printf("Found %d expired medicine(s).\n", value);
                }

                break;
            }

            case 5: {
                int code, qty;
                char batch[16];

                printf("Enter medicine code: ");

                if (scanf("%d", &code) != 1) {
                    printf("Invalid code.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {
                    }
                    break;
                }

                printf("Enter batch number: ");
                scanf(" %15[^\n]", batch);

                printf("Enter quantity to sell: ");

                if (scanf("%d", &qty) != 1) {
                    printf("Invalid quantity.\n");
                    int ch;
                    while ((ch = getchar()) != '\n' && ch != EOF) {
                    }
                    break;
                }

                if (sellMedicine(record, currentSize,
                                 code, batch, qty)) {
                    printf("Sold successfully.\n");
                } else {
                    printf("Sale failed: medicine not found, "
                           "invalid quantity, or insufficient stock.\n");
                }

                break;
            }

            case 6: {
                int value = saveRegistryToFile(
                    record, currentSize, "saved.txt"
                );

                if (value == -1) {
                    printf("File was not saved successfully.\n");
                } else {
                    printf("Saved %d record(s).\n", value);
                }

                break;
            }

            case 7: {
                int value = loadRegistryFromFile(
                    record, MAX, "saved.txt"
                );

                if (value == -1) {
                    printf("Load failed. Existing data is unchanged.\n");
                    break;
                }

                currentSize = value;
                printf("Loaded %d record(s) from file.\n", value);
                break;
            }

            case 0:
                printf("Goodbye!\n");
                break;

            default:
                printf("Invalid menu choice.\n");
        }

    } while (c != 0);

    return 0;
}
