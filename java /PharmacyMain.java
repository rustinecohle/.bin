
import java.util.*;

public class PharmacyMain {

    static final int Max = 150;
    static Scanner sc = new Scanner(System.in);

    static class Medicine {
        int code;
        String name;
        String batch;
        int quantity;
        float price;
        int year;

        Medicine(int code, String name, String batch,
                 int quantity, float price, int year) {
            this.code = code;
            this.name = name;
            this.batch = batch;
            this.quantity = quantity;
            this.price = price;
            this.year = year;
        }
    }

    // Input medicine details
    static Medicine promptMedicineDetails() {

        System.out.print("ENTER CODE: ");
        int code = sc.nextInt();
        sc.nextLine();

        System.out.print("ENTER NAME: ");
        String name = sc.nextLine();

        System.out.print("Enter batch number: ");
        String batch = sc.nextLine();

        System.out.print("Enter quantity: ");
        int quantity = sc.nextInt();

        System.out.print("Enter price: ");
        float price = sc.nextFloat();

        System.out.print("Enter expiry year: ");
        int year = sc.nextInt();
        sc.nextLine();

        return new Medicine(code, name, batch,
                            quantity, price, year);
    }

    // Display one medicine
    static void displayMedicine(Medicine med) {

        System.out.println("----------------------------");
        System.out.println("Code     : " + med.code);
        System.out.println("Name     : " + med.name);
        System.out.println("Batch    : " + med.batch);
        System.out.println("Quantity : " + med.quantity);
        System.out.printf("Price    : %.2f%n", med.price);
        System.out.println("Expiry   : " + med.year);
    }

    // Check duplicate medicine
    static boolean isDuplicate(ArrayList<Medicine> registry,
                               Medicine med) {

        for (Medicine m : registry) {

            if (m.code == med.code &&
                m.batch.equals(med.batch)) {

                return true;
            }
        }

        return false;
    }

    // Find all batches with given code
    static int findMedicinesByCode(ArrayList<Medicine> registry,
                                   int searchCode) {

        int count = 0;

        for (Medicine med : registry) {

            if (med.code == searchCode) {
                displayMedicine(med);
                count++;
            }
        }

        return count;
    }

    // Display expired medicines
    static int getExpiredList(ArrayList<Medicine> registry,
                              int currentYear) {

        int count = 0;

        for (Medicine med : registry) {

            if (med.year < currentYear) {
                displayMedicine(med);
                count++;
            }
        }

        return count;
    }

    // Find medicine by code and batch
    static int searchByCodeBatch(ArrayList<Medicine> registry,
                                 int code, String batch) {

        for (int i = 0; i < registry.size(); i++) {

            Medicine med = registry.get(i);

            if (med.code == code &&
                med.batch.equals(batch)) {

                return i;
            }
        }

        return -1;
    }

    // Sell medicine
    static boolean sellMedicine(ArrayList<Medicine> registry,
                                int code, String batch, int qty) {

        if (qty <= 0) {
            return false;
        }

        int index = searchByCodeBatch(registry, code, batch);

        if (index == -1 ||
            qty > registry.get(index).quantity) {

            return false;
        }

        registry.get(index).quantity -= qty;

        return true;
    }

    public static void main(String[] args) {

        ArrayList<Medicine> record = new ArrayList<>();

        int c;

        do {

            System.out.println("\n===== PHARMACY INVENTORY =====");
            System.out.println("1. Add New Medicine");
            System.out.println("2. Search by Medicine Code");
            System.out.println("3. Display All");
            System.out.println("4. Expired Medicines List");
            System.out.println("5. Sell Medicine");
            System.out.println("0. Exit");

            System.out.print("Choice: ");
            c = sc.nextInt();
            sc.nextLine();

            switch (c) {

                case 1: {

                    if (record.size() >= Max) {
                        System.out.println("Registry is full!");
                        break;
                    }

                    Medicine med = promptMedicineDetails();

                    if (isDuplicate(record, med)) {
                        System.out.println("Medicine already exists!");
                    } else {
                        record.add(med);
                        System.out.println("Medicine added successfully!");
                    }

                    break;
                }

                case 2: {

                    System.out.print("Enter medicine code: ");
                    int code = sc.nextInt();
                    sc.nextLine();

                    int count = findMedicinesByCode(record, code);

                    if (count == 0) {
                        System.out.println("Medicine not found!");
                    }

                    break;
                }

                case 3: {

                    if (record.isEmpty()) {
                        System.out.println("No medicines available!");
                    } else {
                        for (Medicine med : record) {
                            displayMedicine(med);
                        }
                    }

                    break;
                }

                case 4: {

                    System.out.print("Enter current year: ");
                    int currentYear = sc.nextInt();
                    sc.nextLine();

                    int count = getExpiredList(record, currentYear);

                    if (count == 0) {
                        System.out.println("No expired medicines!");
                    }

                    break;
                }

                case 5: {

                    System.out.print("Enter medicine code: ");
                    int code = sc.nextInt();
                    sc.nextLine();

                    System.out.print("Enter batch number: ");
                    String batch = sc.nextLine();

                    System.out.print("Enter quantity to sell: ");
                    int qty = sc.nextInt();
                    sc.nextLine();

                    if (sellMedicine(record, code, batch, qty)) {
                        System.out.println("Medicine sold successfully!");
                    } else {
                        System.out.println("Sale failed! Check medicine or quantity.");
                    }

                    break;
                }

                case 0:
                    System.out.println("Exiting program...");
                    break;

                default:
                    System.out.println("Invalid choice!");
            }

        } while (c != 0);

        sc.close();
    }
}
