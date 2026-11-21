package javaStruct;

// public class StudentIDCard {
    
// }
public class StudentIDCard {
    private String studentName;
    private String studentId;
    private double printBalance;
    private boolean isActive;

    StudentIDCard(String name, String id, double initBalance) {
        studentName = name;
        studentId = id;

        if (initBalance >= 0) {
            printBalance = initBalance;
        } else {
            printBalance = 0;
        }

        isActive = true;
    }

    void topUpBalance(double amount) {
        if (isActive && amount > 0) {
            printBalance += amount;
        } else {
            System.out.println("ERROR");
        }
    }

    boolean deductPrintFee(double cost) {
        if (isActive && cost > 0 && cost <= printBalance) {
            printBalance -= cost;
            return true;
        } else {
            System.out.println("ERROR");
            return false;
        }
    }

    void tapToEnter() {
        if (isActive) {
            System.out.println("Access granted: " + studentName);
        } else {
            System.out.println("Access denied");
        }
    }

    void deactivate() {
        isActive = false;
    }

    String displayCardDetails() {
        String status;
        if (isActive) {
            status = "Active";
        } else {
            status = "Inactive";
        }

        return "Student: " + studentName +
               "\nID: " + studentId +
               "\nBalance: $" + printBalance +
               "\nStatus: " + status;
    }
}
