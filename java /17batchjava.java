
import java.util.Scanner;

class Reservation {
    int id;
    String name;
    int no;
    String type;
    String date;
    int rent;
    int status;
}

public class 17batchjava {

    static final int MAX = 10;
    static Scanner sc = new Scanner(System.in);

    static Reservation addNew() {
        Reservation roomPtr = new Reservation();

        System.out.println("1.enter id");
        roomPtr.id = sc.nextInt();
        sc.nextLine();

        System.out.println("2.enter name");
        roomPtr.name = sc.nextLine();

        System.out.println("3.enter number");
        roomPtr.no = sc.nextInt();
        sc.nextLine();

        System.out.println("4.enter type");
        roomPtr.type = sc.nextLine();

        System.out.println("5.enter date");
        roomPtr.date = sc.nextLine();

        System.out.println("6.enter rent");
        roomPtr.rent = sc.nextInt();

        roomPtr.status = 1;

        return roomPtr;
    }

    static void display(Reservation[] registry, int index) {
        System.out.println("id " + registry[index].id);
        System.out.println("name " + registry[index].name);
        System.out.println("room number " + registry[index].no);
        System.out.println("room type " + registry[index].type);
        System.out.println("date " + registry[index].date);
        System.out.println("rent " + registry[index].rent);
        System.out.println("status " + registry[index].status);
    }

    static int searchByID(Reservation[] registry, int currentSize, int id) {

        for (int i = 0; i < currentSize; i++) {
            if (registry[i].id == id) {
                return i;
            }
        }

        return -1;
    }

    static int searchByDate(Reservation[] registry, int currentSize, String date) {

        for (int i = 0; i < currentSize; i++) {
            if (registry[i].date.equals(date)) {
                return i;
            }
        }

        return -1;
    }

    static void vacant(Reservation[] registry, int size) {

        for (int roomNo = 1; roomNo <= MAX; roomNo++) {

            int occupied = 0;

            for (int i = 0; i < size; i++) {

                if (registry[i].no == roomNo && registry[i].status == 1) {
                    occupied = 1;
                    break;
                }
            }

            if (occupied == 0) {
                System.out.println("room number " + roomNo + " is vacant");
            }
        }
    }

    static int revenue(Reservation[] registry, int currentSize) {

        int total = 0;

        for (int i = 0; i < currentSize; i++) {
            total += registry[i].rent;
        }

        return total * 365;
    }

    public static void main(String[] args) {

        Reservation[] room = new Reservation[MAX];

        int currentSize = 0;
        int c;

        do {

            System.out.println("\n1.add new reservation");
            System.out.println("2.search reservation by ID");
            System.out.println("3.Display all reservation by date");
            System.out.println("4.Display vacant room");
            System.out.println("5.calculate total yearly revenue");
            System.out.println("0.exit");

            c = sc.nextInt();

            switch (c) {

                case 1: {

                    if (currentSize == MAX) {
                        System.out.println("reservation registry is full");
                        break;
                    }

                    Reservation temp = addNew();

                    room[currentSize] = temp;
                    currentSize++;

                    System.out.println("room " + currentSize + " added successfully");

                    break;
                }

                case 2: {

                    int id;

                    System.out.println("enter ID");
                    id = sc.nextInt();

                    int value = searchByID(room, currentSize, id);

                    if (value == -1) {
                        System.out.println("no rooms found");
                        break;
                    }

                    display(room, value);

                    break;
                }

                case 3: {

                    String date;

                    System.out.println("enter date");
                    date = sc.next();

                    int value = searchByDate(room, currentSize, date);

                    if (value == -1) {
                        System.out.println("no rooms found");
                        break;
                    }

                    display(room, value);

                    break;
                }

                case 4: {

                    vacant(room, currentSize);

                    break;
                }

                case 5: {

                    int value = revenue(room, currentSize);

                    System.out.println("yearly revenue is " + value);

                    break;
                }

                case 0:

                    System.out.println("goodbye");

                    break;

                default:

                    System.out.println("invalid choice");

                    break;
            }

        } while (c != 0);

        sc.close();
    }
}
