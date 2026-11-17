
import java.util.Scanner;
import java.util.LinkedList;

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

        LinkedList<Account> registry = new LinkedList<>();

        int choice;

        do {
            System.out.println("\n1. Add Account");
            System.out.println("2. Display Accounts");
            System.out.println("3. Search Account");
            System.out.println("4. Deposit");
            System.out.println("5. Delete Account");
            System.out.println("0. Exit");

            System.out.print("Enter choice: ");
            choice = sc.nextInt();

            switch (choice) {

                case 1: {

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

                    Account newAccount =
                        new Account(accNo, name, type, balance, year);

                    registry.add(newAccount);

                    System.out.println("Account added!");

                    break;
                }

                case 2: {

                    if (registry.isEmpty()) {
                        System.out.println("No accounts");
                        break;
                    }

                    for (Account temp : registry) {
                        temp.display();
                    }

                    break;
                }

                case 3: {

                    sc.nextLine();

                    System.out.print("Enter account number: ");
                    String search = sc.nextLine();

                    boolean found = false;

                    for (Account temp : registry) {

                        if (temp.accNo.equals(search)) {
                            temp.display();
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

                    for (Account temp : registry) {

                        if (temp.accNo.equals(search)) {

                            System.out.print("Deposit amount: ");
                            double amount = sc.nextDouble();

                            if (amount > 0) {
                                temp.balance += amount;
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

                case 5: {

                    sc.nextLine();

                    System.out.print("Enter account number: ");
                    String search = sc.nextLine();

                    boolean removed = false;

                    for (Account temp : registry) {

                        if (temp.accNo.equals(search)) {
                            registry.remove(temp);
                            removed = true;
                            break;
                        }
                    }

                    if (removed) {
                        System.out.println("Account deleted");
                    } else {
                        System.out.println("Account not found");
                    }

                    break;
                }
            }

        } while (choice != 0);

        sc.close();
    }
}
