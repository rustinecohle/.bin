//without the file I/O
package javaStruct;
// import java.util.i*;
import java.util.Scanner;
public class universityHall {
    static class Hall{
        int room;
        String name;
        String seatLable;
        String roomType;
        float monthlyRent;
        int lastPaidYear; 
    }
    static final int MAX=1500;
    static Scanner sc= new Scanner(System.in);

    static void addNew(Hall room){
        System.out.println("enetr room number:");
        room.room=sc.nextInt();
        sc.nextLine();
        System.out.println("enetr name");
        room.name=sc.nextLine();
        System.out.println("enter steat label");
        room.seatLable=sc.nextLine();
        System.out.println("enter room type ");
        room.roomType=sc.nextLine();
        System.out.println("enter monthly rent");
        room.monthlyRent=sc.nextInt();
        System.out.println("emnter last paid year");
        room.lastPaidYear=sc.nextInt();

    }
    static void display(Hall room){
        System.out.println("room id is "+room.room);
        System.out.println("owner is "+room.name);
        System.out.println("seat label is "+room.seatLable);
        System.out.println("room type is "+room.roomType);
        System.out.println("monthly rent is "+room.monthlyRent);
        System.out.println("last paid year is "+room.lastPaidYear);
        System.out.println("===========================");
    }
    static int rentDefaulter(Hall[] registry, int currentSize, int year){
        int count=0;
        for(int i=0; i<currentSize; i++){
            if((year-registry[i].lastPaidYear)>=2){
                count++;
                display(registry[i]);
            }
        }
        return count;
    }
    static int searchByID(Hall[] registry, int currentSize, int id){
        for(int i=0; i<currentSize; i++){
            if(registry[i].room==id){
                return i;
            }
        }
        return -1;
    }
    static boolean vacate(Hall[] registry, int currentSize, int id){
        int index=searchByID(registry, currentSize, id);
        if(index==-1){
            System.out.println("could not find the room ");
            return false;
        }
        for(int i=index; i<currentSize-1; i++){
            registry[i]=registry[i+1];
        }
        return true;
    }
    public static void main(String[] args) {
        Hall[] registry=new Hall[MAX];
        int currentSize=0;
        int c;
        System.out.println("\n=======UNIVERSITY HALL ROOM========");
        do{
            System.out.println("1.add new seat");
            System.out.println("2.search records by book numbers");
            System.out.println("3.display all boooks");
            System.out.println("4.generate rent defaulters ");
            System.out.println("5.vacate a seat");
            System.out.println("6.file saving is not available right now");
            System.out.println("0.exit");
            System.out.println("enter choice");
            c=sc.nextInt();
            switch(c){
                case 1:{
                    Hall temp=new Hall();
                    if(currentSize>=MAX){
                        System.out.println("MAX size reached");
                        break;
                    }
                    int duplicate=0;
                    addNew(temp);
                    for(int i=0; i<currentSize;i++){
                        if(registry[i].room==temp.room){
                            duplicate=1;
                        }
                    }
                    // addNew(temp);

                    if(duplicate==0){
                        registry[currentSize++]=temp;
                    }
                    else System.out.println("duplicate room id found!!!");
                    break;
                }
                case 2: {
                    // if(currentSize)
                    System.out.println("enter room id to search ");

                    int search=sc.nextInt();
                    int count=0;
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].room==search){
                            count++;
                            display(registry[i]);
                        }
                    }
                    System.out.println("found "+count+"rooms");
                    break;
                    
                }
                case 3: 
                {
                    if(currentSize==0){
                        System.out.println("registry is empty");break;
                    }
                    else {
                        for(int i=0; i<currentSize; i++){
                            display(registry[i]);
                        }
                    }
                    break;
                }
                case 4:{
                    int thisYear=sc.nextInt();
                    System.out.println("enter current year ");
                    int value=rentDefaulter(registry, currentSize, thisYear);
                    if (value > 0) {
                        System.out.println("found " + value + " defaulters");
                    } else {
                        System.out.println("no defaulters");
                        }
                    break;
                }
                case 5: {
                    int id=sc.nextInt();
                    System.out.println("enetr room id to vacate ");
                    boolean value=vacate(registry, currentSize, id);
                    if(value){
                        System.out.println("Room number "+id+"vacated successfully");
                    }
                    else System.out.println("could not delete");
                    break;
                }
            }
        }while(c!=0);
        sc.close();
    }
}
