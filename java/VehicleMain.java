package javaStruct.oop;
import java.util.Scanner;
public class VehicleMain {
    static final int MAX=150;
    static Scanner sc=new Scanner(System.in);
    public static void main(String[] args) {
        Vehicle[] registry=new Vehicle[MAX];
        int currentSize=0;
        int c;
        do{
            System.out.println("\n1. Register New Vehicle");
            System.out.println("2. Search by Plate Number");
            System.out.println("3. Show Vehicles of an Owner");
            System.out.println("4. Display All Records");
            System.out.println("5. Tax Token Expired List");
            System.out.println("6. Transfer Ownership");
            System.out.println("7. Deregister a Vehicle");
            System.out.println("0.Exit");
            System.out.println("enter your choice : ");
            c = sc.nextInt();
            switch(c){
                case 1:{
                    if(currentSize>=MAX){
                        System.out.println("max size reached!");
                        break;
                    }
                    System.out.println("enter plate number");
                    String plate=sc.next();
                    System.out.println("enter owner name");
                    String owner=sc.next();
                    System.out.println("enter vehicle type");
                    String type=sc.next();

                    int regYear;
                    do{
                        System.out.println("enter registration year (1950 to 2100)");
                        regYear=sc.nextInt();
                    }while(regYear<1950 || regYear>2100);

                    int cc;
                    do{
                        System.out.println("enter engine capacity in cc (greater than 0)");
                        cc=sc.nextInt();
                    }while(cc<=0);

                    int taxYear;
                    do{
                        System.out.println("enter last tax token year (not before "+regYear+")");
                        taxYear=sc.nextInt();
                    }while(taxYear<regYear);

                    if(VehicleRegistry.findIndex(registry, currentSize, plate)!=-1){
                        System.out.println("error ! duplicate found");
                        break;
                    }
                    Vehicle temp=new Vehicle(plate, owner, type, regYear, cc, taxYear);
                    registry[currentSize]=temp;
                    currentSize++;
                    System.out.println("registered successfully!");
                    break;
                }
                case 2:{
                    System.out.println("enter plate number");
                    String plate=sc.next();
                    int ind=VehicleRegistry.findIndex(registry, currentSize, plate);
                    if(ind==-1){
                        System.out.println("found 0 results");
                    }
                    else{
                        registry[ind].display();
                        System.out.println("found 1 result");
                    }
                    break;
                }
                case 3:{
                    System.out.println("enter owner name: ");
                    String name=sc.next();
                    int count=VehicleRegistry.showByOwner(registry, currentSize, name);
                    if(count>0){
                        System.out.println("found "+count+" results");
                    }
                    else System.out.println("did not find result");
                    break;
                }
                case 4:{
                    if(currentSize==0){
                        System.out.println("register is empty");
                        break;
                    }
                    for(int i=0; i<currentSize; i++){
                        registry[i].display();
                    }
                    break;
                }
                case 5:{
                    System.out.println("enter current year: ");
                    int year=sc.nextInt();
                    int value=VehicleRegistry.showTaxExpired(registry, currentSize, year);
                    System.out.println("found "+value+" results!");
                    break;
                }
                case 6:{
                    System.out.println("enter plate to search");
                    String plate=sc.next();
                    System.out.println("enter new owner name: ");
                    String name=sc.next();
                    int ind=VehicleRegistry.findIndex(registry, currentSize, plate);
                    if(ind==-1){
                        System.out.println("could not find it");
                    }
                    else{
                        registry[ind].transferTo(name);
                        System.out.println("changed successfully!");
                    }
                    break;
                }
                case 7:{
                    System.out.println("enter plate to search");
                    String plate=sc.next();
                    boolean value=VehicleRegistry.deregister(registry, currentSize, plate);
                    if(value){
                        currentSize--;
                        System.out.println("deregistration successful!!");
                    }
                    else System.out.println("failed to deregister!");
                    break;
                }
                case 0:
                    System.out.println("good bye!");
                    break;
                default:
                    System.out.println("invalid choice");
            }
        }while(c!=0);
        sc.close();
    } 
}
