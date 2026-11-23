package javaStruct.fullOop;
import java.util.Scanner;

public class AccountMain {
    static Scanner sc=new Scanner(System.in);
    static final int MAX=10;
    public static void main(String[] args) {
        Account[] registry=new Account[MAX];
        int currentSize=0;
        int choice;
        do{
            System.out.println("\n===== BANK ACCOUNT REGISTRY =====");
            System.out.println("1. Open Account");
            System.out.println("2. Search by Holder");
            System.out.println("3. Display All");
            System.out.println("4. Deposit");
            System.out.println("5. Withdraw");
            System.out.println("6. Transfer");
            System.out.println("7. Dormant Accounts List");
            System.out.println("8. Close Account");
            System.out.println("0. Exit");

            System.out.print("Enter choice: ");
            choice = sc.nextInt();
            sc.nextLine();
            switch (choice) {
                case 1:{
                        if (currentSize >= MAX) {
                        System.out.println("Registry is full");
                        break;
                    }

                    System.out.print("Account number: ");
                    String accNo = sc.nextLine();

                    System.out.print("Holder name: ");
                    String name = sc.nextLine();

                    System.out.print("Account type (Savings/Current): ");
                    String type = sc.nextLine();

                    System.out.print("Opening balance: ");
                    float balance = sc.nextFloat();

                    System.out.print("Last transaction year: ");
                    int year = sc.nextInt();
                    Account temp=new Account(accNo, name, type, balance, year);
                    boolean duplicate=AccountOperation.duplicate(registry, currentSize, temp);
                    if(duplicate){
                        System.out.println("dup found");
                    }
                    else {
                        registry[currentSize]=temp;
                        currentSize++;

                    }
                    break;}
                case 2:{
                    System.out.println("enter name");
                    String name=sc.next();
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].getName(name)){
                            registry[i].display();
                        }
                    }break;
                }
                case 3:{
                    for(int i=0; i<currentSize; i++){
                        // if(registry[i].getName(name)){
                            registry[i].display();
                        
                    }break;
                }
                case 4:{
                    System.out.println("enter amount");
                    float tk=sc.nextFloat();
                    System.out.println("enter acc number");
                    String acc=sc.next();
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].getAccNo().equals(acc)){
                            registry[i].deposit(tk);
                        }
                    }break;
                }
                case 5:{
                    System.out.println("enter amount");
                    float tk=sc.nextFloat();
                    System.out.println("enter acc number");
                    String acc=sc.next();
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].getBalance()>=tk &&registry[i].getAccNo().equals(acc)){
                        //    if(registry)
                            registry[i].withdraw(tk);
                        }
                        
                    }break;
                }
                case 6:{
                    System.out.print("Source account number: ");
                    String sourceNo=sc.nextLine();
                    System.out.print("Destination account number: ");
                    String destinationNo=sc.nextLine();
                    System.out.print("Transfer amount: ");
                    float amount=sc.nextFloat();
                    sc.nextLine();
                    Account source=null;
                    Account destination=null;
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].getAccNo().equals(sourceNo)){
                            source=registry[i];
                        }
                        if(registry[i].getAccNo().equals(destinationNo)){
                            destination=registry[i];
                        }
                    }
                    if(amount<=0){
                        System.out.println("Amount must be positive");
                    }else if(source==null || destination==null){
                        System.out.println("Account not found");
                    }else if(source==destination){
                        System.out.println("Source and destination must be different");
                    }else if(source.getBalance()<amount){
                        System.out.println("Insufficient balance");
                    }else{
                        source.withdraw(amount);
                        destination.deposit(amount);
                        System.out.println("Transfer successful");
                    }
                    break;
                }
                case 7:{
                    System.out.print("Current year: ");
                    int year=sc.nextInt();
                    sc.nextLine();
                    boolean found=false;
                    for(int i=0; i<currentSize; i++){
                        if(year-registry[i].getLastTransactionYear()>2){
                            registry[i].display();
                            found=true;
                        }
                    }
                    if(!found){
                        System.out.println("No dormant accounts found");
                    }
                    break;
                }
                case 8:{
                    System.out.print("Account number to close: ");
                    String accNo=sc.nextLine();
                    int index=-1;
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].getAccNo().equals(accNo)){
                            index=i;
                            break;
                        }
                    }
                    if(index==-1){
                        System.out.println("Account not found");
                    }else{
                        for(int i=index; i<currentSize-1; i++){
                            registry[i]=registry[i+1];
                        }
                        registry[--currentSize]=null;
                        System.out.println("Account closed");
                    }
                    break;
                }
            
                default:
                    break;
            }
        }while(choice!=0);
        sc.close();
    }
}
