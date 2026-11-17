
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Account {
    char accNo[11];
    char holderName[51];
    char type[10];
    float balance;
    int lastTransactionYear;

    struct Account *link;
} Account;

Account *head = NULL;
Account *tail = NULL;

// ADD ACCOUNT
void addAccount()
{
    Account *newNode = malloc(sizeof(Account));

    if (newNode == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    printf("Enter account number: ");
    scanf("%10s", newNode->accNo);

    printf("Enter holder name: ");
    scanf("%50s", newNode->holderName);

    printf("Enter account type: ");
    scanf("%9s", newNode->type);

    printf("Enter balance: ");
    scanf("%f", &newNode->balance);

    printf("Enter last transaction year: ");
    scanf("%d", &newNode->lastTransactionYear);

    newNode->link = NULL;

    if (head == NULL) {
        head = newNode;
    }
    else {
        tail->link = newNode;
    }

    tail = newNode;

    printf("Account added!\n");
}

// DISPLAY ALL ACCOUNTS
void displayAccounts()
{
    Account *temp = head;

    if (head == NULL) {
        printf("No accounts available\n");
        return;
    }

    while (temp != NULL) {
        printf("\nAccount: %s\n", temp->accNo);
        printf("Holder: %s\n", temp->holderName);
        printf("Type: %s\n", temp->type);
        printf("Balance: %.2f\n", temp->balance);
        printf("Last year: %d\n", temp->lastTransactionYear);

        temp = temp->link;
    }
}

// SEARCH ACCOUNT
Account *findAccount(char accNo[])
{
    Account *temp = head;

    while (temp != NULL) {

        if (strcmp(temp->accNo, accNo) == 0) {
            return temp;
        }

        temp = temp->link;
    }

    return NULL;
}

// DEPOSIT
void deposit()
{
    char accNo[11];
    float amount;

    printf("Enter account number: ");
    scanf("%10s", accNo);

    Account *found = findAccount(accNo);

    if (found == NULL) {
        printf("Account not found\n");
        return;
    }

    printf("Enter deposit amount: ");
    scanf("%f", &amount);

    if (amount <= 0) {
        printf("Invalid amount\n");
        return;
    }

    found->balance += amount;

    printf("Deposit successful\n");
}

// MAIN
int main()
{
    int choice;

    do {
        printf("\n1. Add account\n");
        printf("2. Display accounts\n");
        printf("3. Search account\n");
        printf("4. Deposit\n");
        printf("0. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                addAccount();
                break;

            case 2:
                displayAccounts();
                break;

            case 3: {
                char accNo[11];

                printf("Enter account number: ");
                scanf("%10s", accNo);

                Account *found = findAccount(accNo);

                if (found != NULL) {
                    printf("Holder: %s\n", found->holderName);
                    printf("Balance: %.2f\n", found->balance);
                }
                else {
                    printf("Not found\n");
                }

                break;
            }

            case 4:
                deposit();
                break;
        }

    } while (choice != 0);

    return 0;
}
