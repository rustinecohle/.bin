package javaStruct.fullOop;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
public class SeatOperation {
    static Scanner sc=new Scanner(System.in);
    public static void  addNew(Seat temp){
        System.out.println("enter room ");
        temp.room=sc.nextInt();
        System.out.println("enter nmae");
        temp.name=sc.next();
        System.out.println("enter seat label ");
        temp.seatLabel=sc.next();
        System.out.println("enter room type");
        temp.roomType=sc.next();
        System.out.println("enetr mojnthly rent");
        temp.monthlyRent=sc.nextDouble();
        System.out.println("enter last paid year ");
        temp.lastPaidYear=sc.nextInt();
    }
    public static void display(Seat temp){
        System.out.println("room is "+temp.room);
        
        System.out.println("name is "+temp.name);
        System.out.println("seat label is "+temp.seatLabel);
        System.out.println("room type is "+temp.roomType);
        System.out.println("room rent is "+temp.monthlyRent);
        System.out.println("room last paid year is "+temp.lastPaidYear);
    }
    public static int saveToFile(Seat[] registry , int currentSize, String filename){
        try{
            PrintWriter out=new PrintWriter(new FileWriter(filename));
            int count=0;
            for(int i=0; i<currentSize; i++){
                out.printf("%d|%s|%s|%s|%.2f|%d\n", registry[i].room, registry[i].name
                    , registry[i].seatLabel,registry[i].roomType,registry[i].monthlyRent,registry[i].lastPaidYear
                );
                    count++;
                
            }
            out.close();
            return count;
        }catch (IOException e) {return -1;}
    }

}
