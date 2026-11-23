package javaStruct.fullOop;

import java.util.Scanner;

public class SeatRecordMain {
    static Scanner sc=new Scanner(System.in);
    public static void main(String[] args) {
        SeatRecord v=null;
        int c;
        do{
            System.out.println("\n=========================");
            System.out.println("1.add new record");
            System.out.println("2.display record");
            System.out.println("3.check defaulter");
            System.out.println("4.vacate the seat");
            System.out.println("5.save to file");
            System.out.println("0.exit");
            System.out.println("enter your choice ");
            c=sc.nextInt();
            switch (c) {
                case 1:{
                    if(v!=null){
                        System.out.println("already added a record");break;
                    }
                    System.out.println("enter room numebr");
        int room=sc.nextInt();
        System.out.println("enter name ");
        String name=sc.next();
        System.out.println("enter seat label");
        String seatLabel=sc.next();
        System.out.println("enter room type ");
        String roomType=sc.next();
        System.out.println("enter monthly rent ");
        double monthlyRent=sc.nextDouble();
        System.out.println("enter last paid year ");
        int lastPaidYear=sc.nextInt();
                    // v.addNew();
                    v=new SeatRecord( room, name,  seatLabel,  roomType,  monthlyRent, lastPaidYear);
                    break;
                }
                case 2:{
                    if(v==null){
                        System.out.println("record i s empty");
                    }
                    v.display();
                    break;
                }
                case 3:{
                    System.out.println("enter currentYear ");
                    int year=sc.nextInt();
                    int lastyear=v.getLastYear();
                    if(lastyear-year>=2){
                        System.out.println("defaulter !!!!");
                    }else System.out.println("valid");
                    break;
                }
                case 4:{
                    v=null;
                }          
                // case 5:          
                    //break;
            
                default:
                    break;
            }


        }while(c!=0);

    }
}
