package javaStruct.fullOop;

public class Account {
    private String accNo;
    private String holderName;
    private String type;
    private float balance;
    private int lastTransactionYear;
    Account(String accNo, String holderName, String type, float balance, int lastTransactionYear){
        this.accNo=accNo;
        this.holderName=holderName;
        this.type=type;
        this.balance= balance;
        this.lastTransactionYear=lastTransactionYear;
    }
    public void display(){
        System.out.println("\n==============================");
        System.out.println("Account: " + accNo);
        System.out.println("Holder: " + holderName);
        System.out.println("Type: " + type);
        System.out.printf("Balance: %.2f%n", balance);
        System.out.println("Last transaction year: " + lastTransactionYear);
        System.out.println("==============================");
    }
    public String getAccNo(){
        return accNo;
    }
    public boolean getName(String name){
        return holderName.equals(name);
    }
    public float getBalance(){
        return balance;
    }
    public int getLastTransactionYear(){
        return lastTransactionYear;
    }
    public void deposit(float amount){
        balance+=amount;
        lastTransactionYear=java.time.Year.now().getValue();
    } public void withdraw(float amount){
        balance-=amount;
        lastTransactionYear=java.time.Year.now().getValue();
    }

}
