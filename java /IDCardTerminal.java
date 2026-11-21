package javaStruct;
import java.util.Scanner;
// public class IDCardTerminal {
    
// }
public class IDCardTerminal {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        System.out.println("Enter student name");
        String name = sc.nextLine();

        System.out.println("Enter student ID");
        String id = sc.nextLine();

        System.out.println("Enter initial balance");
        double initBalance = sc.nextDouble();

        StudentIDCard card = new StudentIDCard(name, id, initBalance);

        card.tapToEnter();

        System.out.println("Enter top-up amount");
        double topUp = sc.nextDouble();
        card.topUpBalance(topUp);

        int pagesPrinted = 0;
        for (int i = 0; i < 3; i++) {
            if (card.deductPrintFee(0.50)) {
                pagesPrinted++;
            }
        }

        System.out.println("Pages printed: " + pagesPrinted);
        System.out.println(card.displayCardDetails());

        sc.close();
    }
}
