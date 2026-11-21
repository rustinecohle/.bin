package javaStruct;

import java.util.Scanner;

import javaStruct.vehicleData.Vehicle;

public class vehicleMain {
    static final int MAX=150;
    static Scanner sc=new Scanner(System.in);


    public static void main(String[] args) {
        int currentSize=0;
        Vehicle[]  registry=new Vehicle[MAX];
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
            System.out.println("enetr your choice : ");
            c=sc.nextInt();
            switch(c){
                case 1: {
                    Vehicle temp=new Vehicle() ;
                    if(currentSize>MAX){
                        System.out.println("max size reached!");
                        break;
                    }   
                    boolean duplicate=false;
                    vehicleOperation.addNew(temp);
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].plate.equals(temp.plate)){
                            duplicate=true;
                        }
                    }
                    if(duplicate){
                        System.out.println("error ! duplicate found");
                    }else{
                        registry[currentSize++]=temp;

                    }
                    break;
                }
                case 2:{
                    String plate=sc.next();
                    System.out.println("enter plate number ");
                    int count=0;
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].plate.equals(plate)){
                            vehicleOperation.display(registry[i]);
                            count++;
                        }
                    }
                    System.out.println("found "+count+"results");
                    break;
                }
                case 3: {
                    System.out.println("enter owner name: ");
                    String name=sc.next();
                    boolean found=false;
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].owner.equals(name)){
                            vehicleOperation.display(registry[i]);
                            found=true;
                        }
                    }
                    if(found){
                        System.out.println("found !");
                    }
                    else System.out.println("didnot found result");
                    break;
                }
                case 4:{
                    if(currentSize==0){
                        System.out.println("register is empty");
                        break;
                    }
                    for(int i=0; i<currentSize; i++){
                        vehicleOperation.display(registry[i]);
                    }
                    break;
                }
                case 5: {
                    int year=sc.nextInt();
                    System.out.println("enter currenty year: ");
                    int vlaue=vehicleOperation.expiredList(registry, currentSize, year);
                    System.out.println("found "+vlaue+"results!");
                    break;
                }
                case 6:{
                    System.out.println("enter owner name: ");
                    String name=sc.next();
                    System.out.println("enter plate to search");
                    String plate=sc.next();
                    boolean changed=vehicleOperation.transfer(registry, currentSize,plate, name);
                    if(changed){
                        System.out.println("changed successfully!");

                    }
                    else System.out.println("could not find it");
                    break;
                    


                }
                case 7:{
                    System.out.println("enter plate to search");
                    String plate=sc.next();
                    boolean value=vehicleOperation.delete(registry, currentSize, plate);
                    if(value){
                        System.out.println("deregistration succesfull!!");
                    }
                    else System.out.println("failed to deregister!");
                    break;
                }
            }
        }while(c!=0);
        sc.close();

    }
}
