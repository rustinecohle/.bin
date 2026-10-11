```c
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX 100
#define NAME_LEN 51
#define NO_LEN 11
#define TYPE_LEN 10
#define LINE_LEN 512

typedef struct {
    char accNo[NO_LEN];
    char holderName[NAME_LEN];
    char type[TYPE_LEN];
    float balance;
    int lastTransactionYear;
} Account;

/* ---------- Input helpers ---------- */

void readLine(const char *message, char *str, int size)
{
    int ch;

    printf("%s", message);

    if (fgets(str, size, stdin) == NULL) {
        str[0] = '\0';
        return;
    }

    if (strchr(str, '\n') == NULL) {
        while ((ch = getchar()) != '\n' && ch != EOF)
            ;
    } else {
        str[strcspn(str, "\n")] = '\0';
    }
}

int readInt(const char *message)
{
    char line[100];
    int value;
    char extra;

    while (1) {
        readLine(message, line, sizeof(line));

        if (sscanf(line, " %d %c", &value, &extra) == 1)
            return value;

        printf("Invalid integer. Try again.\n");
    }
}

float readFloat(const char *message)
{
    char line[100];
    float value;
    char extra;

    while (1) {
        readLine(message, line, sizeof(line));

        if (sscanf(line, " %f %c", &value, &extra) == 1)
            return value;

        printf("Invalid number. Try again.\n");
    }
}

/* ---------- Validation ---------- */

int isValidAccountNo(const char *accNo)
{
    int i;

    if (strlen(accNo) != 10)
        return 0;

    for (i = 0; i < 10; i++) {
        if (!isdigit((unsigned char)accNo[i]))
            return 0;
    }

    return 1;
}

int isValidType(const char *type)
{
    return strcmp(type, "Savings") == 0 ||
           strcmp(type, "Current") == 0;
}

float minimumBalance(const char *type)
{
    if (strcmp(type, "Savings") == 0)
        return 500.00f;

    return 0.00f;
}

int findAccountIndex(const Account registry[], int currentSize,
                     const char *accNo)
{
    int i;

    for (i = 0; i < currentSize; i++) {
        if (strcmp(registry[i].accNo, accNo) == 0)
            return i;
    }

    return -1;
}

/* ---------- Input and display ---------- */

void promptAccountDetails(Account *accPtr)
{
    while (1) {
        readLine("Account number (10 digits): ",
                 accPtr->accNo, sizeof(accPtr->accNo));

        if (isValidAccountNo(accPtr->accNo))
            break;

        printf("Invalid account number. Enter exactly 10 digits.\n");
    }

    while (1) {
        readLine("Holder name: ",
                 accPtr->holderName, sizeof(accPtr->holderName));

        if (accPtr->holderName[0] != '\0')
            break;

        printf("Name cannot be empty.\n");
    }

    while (1) {
        readLine("Account type (Savings/Current): ",
                 accPtr->type, sizeof(accPtr->type));

        if (isValidType(accPtr->type))
            break;

        printf("Enter exactly Savings or Current.\n");
    }

    while (1) {
        accPtr->balance = readFloat("Opening balance: ");

        if (accPtr->balance < 0.0f) {
            printf("Balance cannot be negative.\n");
        } else if (accPtr->balance <
                   minimumBalance(accPtr->type)) {
            printf("Savings needs at least 500.00.\n");
        } else {
            break;
        }
    }

    while (1) {
        accPtr->lastTransactionYear =
            readInt("Last transaction year (1990-2100): ");

        if (accPtr->lastTransactionYear >= 1990 &&
            accPtr->lastTransactionYear <= 2100)
            break;

        printf("Year must be from 1990 to 2100.\n");
    }
}

void displayAccount(const Account *accPtr)
{
    printf("Account: %s | Holder: %s | Type: %s | "
           "Balance: %.2f | Last year: %d\n",
           accPtr->accNo,
           accPtr->holderName,
           accPtr->type,
           accPtr->balance,
           accPtr->lastTransactionYear);
}

/* ---------- Open and close ---------- */

int openAccount(Account registry[], int *currentSize)
{
    Account temp;

    if (*currentSize >= MAX) {
        printf("Registry is full.\n");
        return 0;
    }

    promptAccountDetails(&temp);

    if (findAccountIndex(registry, *currentSize, temp.accNo) != -1) {
        printf("Duplicate account number.\n");
        return -1;
    }

    registry[*currentSize] = temp;
    (*currentSize)++;

    return 1;
}

int closeAccount(Account registry[], int *currentSize,
                 const char *accNo)
{
    int index, i;

    index = findAccountIndex(registry, *currentSize, accNo);

    if (index == -1)
        return 0;

    if (registry[index].balance >= 0.005f)
        return -1;

    for (i = index; i < *currentSize - 1; i++)
        registry[i] = registry[i + 1];

    (*currentSize)--;

    return 1;
}

/* ---------- Search and dormant accounts ---------- */

int findAccountsByHolder(Account registry[], int currentSize,
                         const char *holder,
                         Account *results[], int maxResults)
{
    int i, count = 0;

    for (i = 0; i < currentSize && count < maxResults; i++) {
        if (strcmp(registry[i].holderName, holder) == 0) {
            results[count] = &registry[i];
            count++;
        }
    }

    return count;
}

int getDormantList(const Account registry[], int currentSize,
                   int currentYear)
{
    int i, count = 0;

    for (i = 0; i < currentSize; i++) {
        if (currentYear - registry[i].lastTransactionYear >= 3) {
            displayAccount(&registry[i]);
            count++;
        }
    }

    return count;
}

/* ---------- Deposit and withdrawal ---------- */

int deposit(Account registry[], int currentSize,
            const char *accNo, float amount, int year)
{
    int index = findAccountIndex(registry, currentSize, accNo);

    if (index == -1)
        return 0;

    if (amount <= 0.0f)
        return -1;

    registry[index].balance += amount;
    registry[index].lastTransactionYear = year;

    return 1;
}

int withdraw(Account registry[], int currentSize,
             const char *accNo, float amount, int year)
{
    int index = findAccountIndex(registry, currentSize, accNo);

    if (index == -1)
        return 0;

    if (amount <= 0.0f)
        return -1;

    if (registry[index].balance - amount <
        minimumBalance(registry[index].type))
        return -2;

    registry[index].balance -= amount;
    registry[index].lastTransactionYear = year;

    return 1;
}

/* ---------- Transfer ---------- */

int transfer(Account registry[], int currentSize,
             const char *fromNo, const char *toNo,
             float amount, int year)
{
    int fromIndex, toIndex;

    /* Check conditions in the required order. */

    if (amount <= 0.0f)
        return -3;

    if (strcmp(fromNo, toNo) == 0)
        return -2;

    fromIndex = findAccountIndex(registry, currentSize, fromNo);

    if (fromIndex == -1)
        return 0;

    toIndex = findAccountIndex(registry, currentSize, toNo);

    if (toIndex == -1)
        return -1;

    if (registry[fromIndex].balance - amount <
        minimumBalance(registry[fromIndex].type))
        return -4;

    /* All checks passed: now change both accounts. */
    registry[fromIndex].balance -= amount;
    registry[toIndex].balance += amount;

    registry[fromIndex].lastTransactionYear = year;
    registry[toIndex].lastTransactionYear = year;

    return 1;
}

/* ---------- File encoding helpers ---------- */

/*
   Escape '%' as "%25" and '|' as "%7C".
   This prevents a name containing the separator from breaking
   the account line into incorrect fields.
*/

void encodeName(const char *source, char *dest, int size)
{
    int i, j = 0;

    for (i = 0; source[i] != '\0'; i++) {
        const char *replacement = NULL;

        if (source[i] == '%')
            replacement = "%25";
        else if (source[i] == '|')
            replacement = "%7C";

        if (replacement != NULL) {
            if (j + 3 >= size)
                break;

            dest[j++] = replacement[0];
            dest[j++] = replacement[1];
            dest[j++] = replacement[2];
        } else {
            if (j + 1 >= size)
                break;

            dest[j++] = source[i];
        }
    }

    dest[j] = '\0';
}

int decodeName(const char *source, char *dest, int size)
{
    int i = 0, j = 0;

    while (source[i] != '\0') {
        if (source[i] == '%') {
            if (strncmp(&source[i], "%25", 3) == 0) {
                if (j + 1 >= size)
                    return 0;

                dest[j++] = '%';
                i += 3;
            } else if (strncmp(&source[i], "%7C", 3) == 0) {
                if (j + 1 >= size)
                    return 0;

                dest[j++] = '|';
                i += 3;
            } else {
                return 0;
            }
        } else {
            if (j + 1 >= size)
                return 0;

            dest[j++] = source[i++];
        }
    }

    dest[j] = '\0';
    return 1;
}

/* ---------- Save ---------- */

int saveRegistryToFile(const Account registry[], int currentSize,
                       const char *filename)
{
    FILE *fp = fopen(filename, "w");
    int i;
    float total = 0.0f;
    char encodedName[NAME_LEN * 3];

    if (fp == NULL)
        return -1;

    for (i = 0; i < currentSize; i++) {
        encodeName(registry[i].holderName,
                   encodedName, sizeof(encodedName));

        if (fprintf(fp, "%s|%s|%s|%.2f|%d\n",
                    registry[i].accNo,
                    encodedName,
                    registry[i].type,
                    registry[i].balance,
                    registry[i].lastTransactionYear) < 0) {
            fclose(fp);
            return -1;
        }

        total += registry[i].balance;
    }

    if (fprintf(fp, "TOTAL|%.2f\n", total) < 0) {
        fclose(fp);
        return -1;
    }

    if (fclose(fp) != 0)
        return -1;

    return currentSize;
}

/* ---------- Load validation ---------- */

int validLoadedAccount(const Account *acc)
{
    if (!isValidAccountNo(acc->accNo))
        return 0;

    if (acc->holderName[0] == '\0')
        return 0;

    if (!isValidType(acc->type))
        return 0;

    if (acc->balance < 0.0f)
        return 0;

    if (acc->balance < minimumBalance(acc->type))
        return 0;

    if (acc->lastTransactionYear < 1990 ||
        acc->lastTransactionYear > 2100)
        return 0;

    return 1;
}

int loadRegistryFromFile(Account registry[], int maxSize,
                         const char *filename)
{
    FILE *fp = fopen(filename, "r");
    Account temp[MAX];
    int count = 0, lineNo = 0;
    int totalFound = 0, damaged = 0;
    float fileTotal = 0.0f, loadedTotal = 0.0f;
    char line[LINE_LEN];

    if (fp == NULL) {
        printf("Cannot open file.\n");
        return -1;
    }

    if (maxSize < 0 || maxSize > MAX) {
        fclose(fp);
        return -1;
    }

    while (fgets(line, sizeof(line), fp) != NULL) {
        char accNo[NO_LEN];
        char encodedName[NAME_LEN * 3];
        char holderName[NAME_LEN];
        char type[TYPE_LEN];
        float balance;
        int year, pos = 0;
        Account acc;

        lineNo++;

        if (strchr(line, '\n') == NULL && !feof(fp)) {
            int ch;

            while ((ch = fgetc(fp)) != '\n' && ch != EOF)
                ;

            printf("Warning: line %d is too long.\n", lineNo);
            damaged = 1;
            continue;
        }

        if (strncmp(line, "TOTAL|", 6) == 0) {
            if (totalFound ||
                sscanf(line, "TOTAL|%f %n", &fileTotal, &pos) != 1 ||
                line[pos] != '\0') {
                damaged = 1;
            } else {
                totalFound = 1;
            }

            /* The checksum must be the final line. */
            break;
        }

        if (totalFound) {
            damaged = 1;
            break;
        }

        if (sscanf(line, "%10[^|]|%150[^|]|%9[^|]|%f|%d %n",
                   accNo, encodedName, type, &balance, &year,
                   &pos) != 5 || line[pos] != '\0') {
            printf("Warning: invalid line %d.\n", lineNo);
            continue;
        }

        if (!decodeName(encodedName, holderName, sizeof(holderName))) {
            printf("Warning: invalid name on line %d.\n", lineNo);
            continue;
        }

        strcpy(acc.accNo, accNo);
        strcpy(acc.holderName, holderName);
        strcpy(acc.type, type);
        acc.balance = balance;
        acc.lastTransactionYear = year;

        if (!validLoadedAccount(&acc)) {
            printf("Warning: invalid account on line %d.\n", lineNo);
            continue;
        }

        if (count >= maxSize) {
            printf("Warning: capacity exceeded on line %d.\n", lineNo);
            continue;
        }

        if (findAccountIndex(temp, count, acc.accNo) != -1) {
            printf("Warning: duplicate account on line %d.\n", lineNo);
            continue;
        }

        temp[count++] = acc;
        loadedTotal += acc.balance;
    }

    if (ferror(fp))
        damaged = 1;

    /* Anything after TOTAL makes the file invalid. */
    if (totalFound) {
        int ch;

        while ((ch = fgetc(fp)) != EOF) {
            if (!isspace((unsigned char)ch)) {
                damaged = 1;
                break;
            }
        }
    }

    fclose(fp);

    if (!totalFound)
        damaged = 1;

    if (fileTotal - loadedTotal >= 0.01f ||
        fileTotal - loadedTotal <= -0.01f)
        damaged = 1;

    if (damaged) {
        printf("File is damaged. Registry unchanged.\n");
        return -1;
    }

    /* Commit only after all checks succeed. */
    memcpy(registry, temp, (size_t)count * sizeof(Account));

    return count;
}

/* ---------- Main menu ---------- */

int main(void)
{
    Account registry[MAX];
    int currentSize = 0;
    int choice;

    do {
        printf("\n===== BANK ACCOUNT REGISTRY =====\n");
        printf("1. Open Account\n");
        printf("2. Search by Holder\n");
        printf("3. Display All\n");
        printf("4. Deposit\n");
        printf("5. Withdraw\n");
        printf("6. Transfer\n");
        printf("7. Dormant Accounts List\n");
        printf("8. Close Account\n");
        printf("9. Save to File\n");
        printf("10. Load from File\n");
        printf("0. Exit\n");

        choice = readInt("Enter choice: ");

        switch (choice) {
        case 1: {
            int result = openAccount(registry, &currentSize);

            if (result == 1)
                printf("Account opened successfully.\n");
            else if (result == 0)
                printf("Cannot open account: registry full.\n");
            else
                printf("Account number already exists.\n");

            break;
        }

        case 2: {
            char holder[NAME_LEN];
            Account *results[MAX];
            int i, count;

            readLine("Enter exact holder name: ",
                     holder, sizeof(holder));

            count = findAccountsByHolder(
                registry, currentSize, holder, results, MAX);

            if (count == 0) {
                printf("Not found.\n");
            } else {
                for (i = 0; i < count; i++)
                    displayAccount(results[i]);

                printf("Found %d account(s).\n", count);
            }

            break;
        }

        case 3: {
            int i;

            if (currentSize == 0)
                printf("No accounts.\n");

            for (i = 0; i < currentSize; i++)
                displayAccount(&registry[i]);

            break;
        }

        case 4: {
            char accNo[NO_LEN];
            float amount;
            int year, result;

            readLine("Account number: ", accNo, sizeof(accNo));
            amount = readFloat("Deposit amount: ");
            year = readInt("Current year: ");

            result = deposit(registry, currentSize,
                             accNo, amount, year);

            if (result == 1)
                printf("Deposit successful.\n");
            else if (result == 0)
                printf("Account not found.\n");
            else
                printf("Amount must be greater than zero.\n");

            break;
        }

        case 5: {
            char accNo[NO_LEN];
            float amount;
            int year, result;

            readLine("Account number: ", accNo, sizeof(accNo));
            amount = readFloat("Withdrawal amount: ");
            year = readInt("Current year: ");

            result = withdraw(registry, currentSize,
                              accNo, amount, year);

            if (result == 1)
                printf("Withdrawal successful.\n");
            else if (result == 0)
                printf("Account not found.\n");
            else if (result == -1)
                printf("Amount must be greater than zero.\n");
            else if (result == -2)
                printf("Minimum balance rule violated.\n");

            break;
        }

        case 6: {
            char fromNo[NO_LEN], toNo[NO_LEN];
            float amount;
            int year, result;

            readLine("Sender account number: ",
                     fromNo, sizeof(fromNo));
            readLine("Receiver account number: ",
                     toNo, sizeof(toNo));
            amount = readFloat("Transfer amount: ");
            year = readInt("Current year: ");

            result = transfer(registry, currentSize,
                              fromNo, toNo, amount, year);

            if (result == 1)
                printf("Transfer successful.\n");
            else if (result == 0)
                printf("Sender account not found.\n");
            else if (result == -1)
                printf("Receiver account not found.\n");
            else if (result == -2)
                printf("Cannot transfer to the same account.\n");
            else if (result == -3)
                printf("Amount must be greater than zero.\n");
            else if (result == -4)
                printf("Sender minimum balance would be violated.\n");

            break;
        }

        case 7: {
            int year, count;

            year = readInt("Enter current year: ");

            count = getDormantList(registry, currentSize, year);

            printf("Dormant accounts: %d\n", count);
            break;
        }

        case 8: {
            char accNo[NO_LEN];
            int result;

            readLine("Account number to close: ",
                     accNo, sizeof(accNo));

            result = closeAccount(registry, &currentSize, accNo);

            if (result == 1)
                printf("Account closed.\n");
            else if (result == 0)
                printf("Account not found.\n");
            else
                printf("Balance must be zero before closing.\n");

            break;
        }

        case 9: {
            char filename[100];
            int result;

            readLine("Filename to save (e.g. bank.txt): ",
                     filename, sizeof(filename));

            result = saveRegistryToFile(
                registry, currentSize, filename);

            if (result == -1)
                printf("Could not save file.\n");
            else
                printf("%d account(s) saved.\n", result);

            break;
        }

        case 10: {
            char filename[100];
            int result;

            readLine("Filename to load (e.g. bank.txt): ",
                     filename, sizeof(filename));

            result = loadRegistryFromFile(
                registry, MAX, filename);

            if (result == -1)
                printf("Load failed; existing data unchanged.\n");
            else {
                currentSize = result;
                printf("%d account(s) loaded.\n", result);
            }

            break;
        }

        case 0:
            printf("Goodbye.\n");
            break;

        default:
            printf("Invalid menu choice.\n");
        }

    } while (choice != 0);

    return 0;
}
```
