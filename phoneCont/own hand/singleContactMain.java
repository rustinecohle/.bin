package javaStruct.fullOop;

import java.util.Scanner;

public class singleContactMain {
    static Scanner sc=new Scanner(System.in);
    public static void main(String[] args) {
        singleContact c=null;
        // int currentSize=0;
        int x;
        do{
            System.out.println("\n===============");
            System.out.println("1.add contact");
            System.out.println("2.display ");
            System.out.println("0.exit");
            System.out.println("enter choice ");
            x=sc.nextInt();
            switch(x){
                case 1:{
                    if(c!=null){
                        System.out.println("already registered!");
                        break;
                    }
                    System.out.println("enter name");
                    String name=sc.next();
                    System.out.println("enter number");
                    String number=sc.next();
                    System.out.println("enter email ");
                    String email=sc.next();
                    System.out.println("enter group ");
                    String group=sc.next();
                    System.out.println("enetr year ");
                    int year=sc.nextInt();
                    c=new singleContact(name, number, email, group, year);
                    System.out.println("contact added");

                    break;
                }
                case 2:{
                    c.display();
                }
            }
        }while(x!=0);
        sc.close();
    }
}
