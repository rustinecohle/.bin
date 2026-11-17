
import java.util.Scanner;

class Account {
    String accNo;
    String holderName;
    String type;
    double balance;
    int lastTransactionYear;

    Account(String accNo, String holderName, String type,
            double balance, int year) {

        this.accNo = accNo;
        this.holderName = holderName;
        this.type = type;
        this.balance = balance;
        this.lastTransactionYear = year;
    }

    void display() {
        System.out.println("Account: " + accNo);
        System.out.println("Holder: " + holderName);
        System.out.println("Type: " + type);
        System.out.println("Balance: " + balance);
        System.out.println("Last year: " + lastTransactionYear);
    }
}

public class Main {

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        Account[] registry = new Account[100];
        int currentSize = 0;

        int choice;

        do {
            System.out.println("\n1. Add Account");
            System.out.println("2. Display Accounts");
            System.out.println("3. Search Account");
            System.out.println("4. Deposit");
            System.out.println("0. Exit");

            System.out.print("Enter choice: ");
            choice = sc.nextInt();

            switch (choice) {

                case 1: {

                    if (currentSize == 100) {
                        System.out.println("Registry full");
                        break;
                    }

                    sc.nextLine();

                    System.out.print("Account number: ");
                    String accNo = sc.nextLine();

                    System.out.print("Holder name: ");
                    String name = sc.nextLine();

                    System.out.print("Account type: ");
                    String type = sc.nextLine();

                    System.out.print("Balance: ");
                    double balance = sc.nextDouble();

                    System.out.print("Last transaction year: ");
                    int year = sc.nextInt();

                    registry[currentSize] =
                        new Account(accNo, name, type, balance, year);

                    currentSize++;

                    System.out.println("Account added!");
                    break;
                }

                case 2: {

                    for (int i = 0; i < currentSize; i++) {
                        registry[i].display();
                    }

                    break;
                }

                case 3: {

                    sc.nextLine();

                    System.out.print("Enter account number: ");
                    String search = sc.nextLine();

                    boolean found = false;

                    for (int i = 0; i < currentSize; i++) {

                        if (registry[i].accNo.equals(search)) {
                            registry[i].display();
                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        System.out.println("Account not found");
                    }

                    break;
                }

                case 4: {

                    sc.nextLine();

                    System.out.print("Enter account number: ");
                    String search = sc.nextLine();

                    boolean found = false;

                    for (int i = 0; i < currentSize; i++) {

                        if (registry[i].accNo.equals(search)) {

                            System.out.print("Deposit amount: ");
                            double amount = sc.nextDouble();

                            if (amount > 0) {
                                registry[i].balance += amount;
                                System.out.println("Deposit successful");
                            }

                            found = true;
                            break;
                        }
                    }

                    if (!found) {
                        System.out.println("Account not found");
                    }

                    break;
                }
            }

        } while (choice != 0);

        sc.close();
    }
}
