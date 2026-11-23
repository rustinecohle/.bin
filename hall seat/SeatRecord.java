package javaStruct.fullOop;
import java.util.Scanner;
public class SeatRecord {
    static Scanner sc=new Scanner(System.in);
    private int room;
    private String name;
    private String seatLabel;
    private String roomType;
    private double monthlyRent;
    private int lastPaidYear;
    SeatRecord(int room, String name, String seatLabel, String roomType, double monthlyRent, int lastPaidYear){
        this.room=room;
        this.name=name;
        this.roomType=roomType;
        this.monthlyRent=monthlyRent;
        this.lastPaidYear=lastPaidYear;
    }  
    // public void addNew(SeatRecord v){
        
    //     // boolean x=SeatRecord(room, name,seatLabel, roomType, monthlyRent, lastPaidYear);
    // }
    public int getLastYear(){
        return lastPaidYear;
    }
    public void display(){
        System.out.println("\n===================");
        System.out.println("room number is "+room);
        System.out.println("name  is "+name);
        System.out.println("seat label is "+seatLabel);
        System.out.println("room type is "+roomType);
        System.out.println("monthly rent is "+monthlyRent);
        System.out.println("last paid year is "+lastPaidYear);
    }

}
