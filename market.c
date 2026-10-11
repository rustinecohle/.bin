```c
#include <stdio.h>
#include <string.h>

#define MAX 200
#define FILE_NAME "market.txt"

typedef struct {
    int blockNumber;
    char traderName[101];
    char stallCode[11];
    float area;
    char tradeType[21];
    int lastPaymentYear;
} Stall;

void promptStallDetails(Stall *stallPtr) {
    printf("Enter block number: ");
    scanf("%d", &stallPtr->blockNumber);

    printf("Enter trader name: ");
    scanf(" %100[^\n]", stallPtr->traderName);

    printf("Enter stall code: ");
    scanf("%10s", stallPtr->stallCode);

    printf("Enter area: ");
    scanf("%f", &stallPtr->area);

    printf("Enter trade type: ");
    scanf(" %20[^\n]", stallPtr->tradeType);

    printf("Enter last lease payment year: ");
    scanf("%d", &stallPtr->lastPaymentYear);
}

void displayStall(const Stall *stallPtr) {
    printf("Block number: %d\n", stallPtr->blockNumber);
    printf("Trader name: %s\n", stallPtr->traderName);
    printf("Stall code: %s\n", stallPtr->stallCode);
    printf("Area: %.2f sq ft\n", stallPtr->area);
    printf("Trade type: %s\n", stallPtr->tradeType);
    printf("Last payment year: %d\n",
           stallPtr->lastPaymentYear);
    printf("-------------------------\n");
}

int isDuplicate(const Stall registry[], int currentSize,
                int blockNumber, const char *stallCode) {
    for (int i = 0; i < currentSize; i++) {
        if (registry[i].blockNumber == blockNumber &&
            strcmp(registry[i].stallCode, stallCode) == 0) {
            return 1;
        }
    }

    return 0;
}

int findStallsByBlock(Stall registry[], int currentSize,
                      int searchBlock, Stall *results[],
                      int maxResults) {
    int count = 0;

    for (int i = 0; i < currentSize && count < maxResults; i++) {
        if (registry[i].blockNumber == searchBlock) {
            results[count] = &registry[i];
            count++;
        }
    }

    return count;
}

int getLeaseDefaulterList(const Stall registry[],
                          int currentSize, int currentYear) {
    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (registry[i].lastPaymentYear < currentYear - 1) {
            displayStall(&registry[i]);
            count++;
        }
    }

    if (count == 0) {
        printf("No lease defaulters found.\n");
    }

    return count;
}

int saveRegistryToFile(const Stall registry[],
                       int currentSize, const char *filename) {
    FILE *fp = fopen(filename, "w");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    int count = 0;

    for (int i = 0; i < currentSize; i++) {
        if (fprintf(fp, "%d|%100[^|]|%10[^|]|%.2f|%20[^|]|%d\n",
                    registry[i].blockNumber,
                    registry[i].traderName,
                    registry[i].stallCode,
                    registry[i].area,
                    registry[i].tradeType,
                    registry[i].lastPaymentYear) < 0) {
            fclose(fp);
            printf("Error writing file.\n");
            return -1;
        }

        count++;
    }

    if (fclose(fp) != 0) {
        printf("Error closing file after saving.\n");
        return -1;
    }

    printf("Saved %d stall record(s).\n", count);
    return count;
}

int loadRegistryFromFile(Stall registry[], int maxSize,
                         const char *filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        printf("Error opening file.\n");
        return -1;
    }

    char line[300];
    int count = 0;
    int lineNo = 0;

    while (fgets(line, sizeof(line), fp) != NULL) {
        lineNo++;

        if (count >= maxSize) {
            printf("Registry full. Remaining records ignored.\n");
            break;
        }

        Stall temp;

        int r = sscanf(line, "%d|%100[^|]|%10[^|]|%f|%20[^|]|%d",
                       &temp.blockNumber,
                       temp.traderName,
                       temp.stallCode,
                       &temp.area,
                       temp.tradeType,
                       &temp.lastPaymentYear);

        if (r != 6) {
            printf("Skipping invalid line %d.\n", lineNo);
            continue;
        }

        registry[count] = temp;
        count++;
    }

    if (ferror(fp)) {
        fclose(fp);
        printf("Error reading file.\n");
        return -1;
    }

    fclose(fp);

    printf("Loaded %d stall record(s).\n", count);
    return count;
}

int main(void) {
    Stall registry[MAX];
    int currentSize = 0;
    int choice;

    do {
        printf("\n--- MARKET STALL REGISTRY ---\n");
        printf("1. Add New Stall Record\n");
        printf("2. Search Records by Block Number\n");
        printf("3. Display All Records\n");
        printf("4. Generate Lease Defaulter List\n");
        printf("5. Save Registry to File\n");
        printf("6. Load Registry from File\n");
        printf("0. Exit\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
            printf("Invalid input.\n");
            continue;
        }

        switch (choice) {
            case 1: {
                if (currentSize >= MAX) {
                    printf("Registry is full.\n");
                    break;
                }

                Stall temp;
                promptStallDetails(&temp);

                if (isDuplicate(registry, currentSize,
                                temp.blockNumber,
                                temp.stallCode)) {
                    printf("Error: Stall already exists in this block.\n");
                    break;
                }

                registry[currentSize] = temp;
                currentSize++;

                printf("Stall added successfully.\n");
                break;
            }

            case 2: {
                int searchBlock;

                printf("Enter block number: ");
                scanf("%d", &searchBlock);

                Stall *results[MAX];

                int value = findStallsByBlock(
                    registry, currentSize, searchBlock,
                    results, MAX
                );

                if (value == 0) {
                    printf("No stalls found.\n");
                    break;
                }

                printf("Found %d stall(s).\n", value);

                for (int i = 0; i < value; i++) {
                    displayStall(results[i]);
                }

                break;
            }

            case 3: {
                if (currentSize == 0) {
                    printf("Registry is empty.\n");
                    break;
                }

                for (int i = 0; i < currentSize; i++) {
                    displayStall(&registry[i]);
                }

                break;
            }

            case 4: {
                int currentYear;

                printf("Enter current year: ");
                scanf("%d", &currentYear);

                int value = getLeaseDefaulterList(
                    registry, currentSize, currentYear
                );

                printf("Total defaulters: %d\n", value);
                break;
            }

            case 5: {
                int value = saveRegistryToFile(
                    registry, currentSize, FILE_NAME
                );

                if (value == -1) {
                    printf("Could not save registry.\n");
                }

                break;
            }

            case 6: {
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
