package javaStruct;

import java.util.Scanner;

import javaStruct.vehicleData.Vehicle;

public class vehicleOperation {
    static Scanner sc=new Scanner(System.in);
    static void addNew(Vehicle temp){
        System.out.println("enter plate s:");
        temp.plate=sc.next();
        System.out.println("enter owner name ");
        temp.owner=sc.next();
        System.out.println("enter type s ");
        temp.type=sc.next();
        System.out.println("enter reg yeae ");
        temp.regYear=sc.nextInt();
        System.out.println("enter cc ");
        temp.cc=sc.nextInt();
        System.out.println("emnter tax year ");
        temp.taxYear=sc.nextInt();    
    }

    static void display(Vehicle temp){
        System.out.println("================results:");
        System.out.println("plate is "+temp.plate);
        System.out.println("owner is "+temp.owner);
        System.out.println("typoe  is "+temp.type);
        System.out.println("registtation year  is "+temp.regYear);
        System.out.println("cc is "+temp.cc);
        System.out.println("tax year is "+temp.taxYear);
        System.out.println("===========================");
    }
    static int expiredList(Vehicle[] registry , int currentSize, int year){
        int count=0;
        for(int i=0; i<currentSize; i++){
            if((year-registry[i].taxYear)>=2){
                display(registry[i]);
                count++;
            }
        }
        return count;
    }
    static boolean transfer(Vehicle[] registry, int currentSize,String plate, String name){
        for(int i=0; i<currentSize; i++){
            if(registry[i].plate.equals(plate)){
                registry[i].owner=name;
                return true;
            }
        }
        return false;
    }
    static int index(Vehicle[] registry, int currentSize, String plate){
        for(int i=0; i<currentSize; i++){
            if(registry[i].plate.equals(plate)){
                return i;
            }
        }
        return -1;
    }
    static boolean delete(Vehicle[] registry, int currentSize, String plate){
        int ind=index(registry, currentSize, plate);
        if(ind==-1){
            System.out.println("could not find the vehicle!");
            return false;
        }
        for(int i=ind; i<currentSize-1; i++ ){
            registry[i]=registry[i+1];
            
        }
        return true;
        
    }

}
