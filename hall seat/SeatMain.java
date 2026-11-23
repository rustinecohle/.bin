package javaStruct.fullOop;

import java.util.Scanner;

public class SeatMain {
    static Scanner sc = SeatOperation.sc;

    public static void main(String[] args) {
        Seat[] registry = new Seat[10];
        int currentSize = 0;
        int choice;

        do {
            System.out.println("\n===== UNIVERSITY HALL SEAT REGISTRY =====");
            System.out.println("1. Add New Seat Record");
            System.out.println("2. Search Records by Room Number");
            System.out.println("3. Display All Records");
            System.out.println("4. Generate Rent Defaulter List");
            System.out.println("5. Vacate a Seat");
            System.out.println("6. Save Registry to File");
            System.out.println("0. Exit");
            System.out.println("Enter choice: ");
            choice = sc.nextInt();

            switch (choice) {
                case 1: {
                    if (currentSize >= registry.length) {
                        System.out.println("Registry is full.");
                        break;
                    }

                    Seat temp = new Seat();
                    SeatOperation.addNew(temp);
                    boolean duplicate = false;
                    for (int i = 0; i < currentSize; i++) {
                        if (registry[i].room == temp.room) {
                            duplicate = true;
                            break;
                        }
                    }

                    if (duplicate) {
                        System.out.println("A record for that room already exists.");
                    } else {
                        registry[currentSize] = temp;
                        currentSize++;
                        System.out.println("Record added.");
                    }
                    break;
                }
                case 2: {
                    if (currentSize == 0) {
                        System.out.println("No records to search.");
                        break;
                    }

                    System.out.println("Enter room number:");
                    int number = sc.nextInt();
                    boolean found = false;
                    for (int i = 0; i < currentSize; i++) {
                        if (registry[i].room == number) {
                            SeatOperation.display(registry[i]);
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        System.out.println("No record found for that room.");
                    }
                    break;
                }
                case 3: {
                    if (currentSize == 0) {
                        System.out.println("No records to display.");
                    } else {
                        for (int i = 0; i < currentSize; i++) {
                            SeatOperation.display(registry[i]);
                        }
                    }
                    break;
                }
                case 4: {
                    if (currentSize == 0) {
                        System.out.println("No records to check.");
                        break;
                    }

                    System.out.println("Enter current year:");
                    int currentYear = sc.nextInt();
                    boolean foundDefaulter = false;
                    for (int i = 0; i < currentSize; i++) {
                        if (currentYear - registry[i].lastPaidYear >= 2) {
                            SeatOperation.display(registry[i]);
                            foundDefaulter = true;
                        }
                    }
                    if (!foundDefaulter) {
                        System.out.println("No rent defaulters found.");
                    }
                    break;
                }
                case 5: {
                    if (currentSize == 0) {
                        System.out.println("No seats to vacate.");
                        break;
                    }

                    System.out.println("Enter room number to vacate:");
                    int number = sc.nextInt();
                    int index = -1;
                    for (int i = 0; i < currentSize; i++) {
                        if (registry[i].room == number) {
                            index = i;
                            break;
                        }
                    }

                    if (index == -1) {
                        System.out.println("No record found for that room.");
                    } else {
                        for (int i = index; i < currentSize - 1; i++) {
                            registry[i] = registry[i + 1];
                        }
                        registry[--currentSize] = null;
                        System.out.println("Seat vacated.");
                    }
                    break;
                }
                case 6: {
                    int saved = SeatOperation.saveToFile(registry, currentSize, "saved.txt");
                    if (saved == -1) {
                        System.out.println("Could not save the registry.");
                    } else {
                        System.out.println("Saved " + saved + " records.");
                    }
                    break;
                }
                default:
                    if (choice != 0) {
                        System.out.println("Invalid choice.");
                    }
                    break;
            }
        } while (choice != 0);

        sc.close();
    }
}
