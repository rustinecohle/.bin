package javaStruct.oop;

import java.util.Scanner;

public class vehicleMain {
    static final int MAX = 150;
    static Scanner sc = new Scanner(System.in);

    // every input is read as a full line, so spaces work and no leftover newline bug
    static String readText(String prompt) {
        System.out.println(prompt);
        return sc.nextLine().trim();
    }

    static int readInt(String prompt) {
        while (true) {
            System.out.println(prompt);
            try {
                return Integer.parseInt(sc.nextLine().trim());
            } catch (NumberFormatException e) {
                System.out.println("please enter a number");
            }
        }
    }

    public static void main(String[] args) {
        VehicleRegistry registry = new VehicleRegistry(MAX);
        int c;
        do {
            System.out.println("\n1. Register New Vehicle");
            System.out.println("2. Search by Plate Number");
            System.out.println("3. Show Vehicles of an Owner");
            System.out.println("4. Display All Records");
            System.out.println("5. Tax Token Expired List");
            System.out.println("6. Transfer Ownership");
            System.out.println("7. Deregister a Vehicle");
            System.out.println("0.Exit");
            c = readInt("enter your choice : ");

            switch (c) {
                case 1: {
                    if (registry.isFull()) {
                        System.out.println("max size reached!");
                        break;
                    }
                    String plate = readText("enter plate number");
                    String owner = readText("enter owner name");
                    String type = readText("enter vehicle type");
                    int regYear = readInt("enter registration year (1950 to 2100)");
                    int cc = readInt("enter engine capacity in cc");
                    int taxYear = readInt("enter last tax token year");
                    try {
                        Vehicle temp = new Vehicle(plate, owner, type, regYear, cc, taxYear);
                        if (registry.add(temp)) {
                            System.out.println("registered successfully!");
                        } else {
                            System.out.println("error ! duplicate found");
                        }
                    } catch (IllegalArgumentException e) {
                        System.out.println("error ! " + e.getMessage());
                    }
                    break;
                }
                case 2: {
                    String plate = readText("enter plate number");
                    Vehicle v = registry.findByPlate(plate);
                    if (v == null) {
                        System.out.println("found 0 results");
                    } else {
                        v.display();
                        System.out.println("found 1 result");
                    }
                    break;
                }
                case 3: {
                    String name = readText("enter owner name: ");
                    int count = registry.showByOwner(name);
                    if (count > 0) {
                        System.out.println("found " + count + " results");
                    } else {
                        System.out.println("did not find result");
                    }
                    break;
                }
                case 4: {
                    if (registry.getSize() == 0) {
                        System.out.println("register is empty");
                        break;
                    }
                    registry.displayAll();
                    break;
                }
                case 5: {
                    int year = readInt("enter current year: ");
                    int value = registry.showTaxExpired(year);
                    System.out.println("found " + value + " results!");
                    break;
                }
                case 6: {
                    String plate = readText("enter plate to search");
                    String name = readText("enter new owner name: ");
                    if (registry.transfer(plate, name)) {
                        System.out.println("changed successfully!");
                    } else {
                        System.out.println("could not find it");
                    }
                    break;
                }
                case 7: {
                    String plate = readText("enter plate to search");
                    if (registry.deregister(plate)) {
                        System.out.println("deregistration successful!!");
                    } else {
                        System.out.println("failed to deregister!");
                    }
                    break;
                }
                case 0:
                    System.out.println("good bye!");
                    break;
                default:
                    System.out.println("invalid choice");
            }
        } while (c != 0);
        sc.close();
    }
}