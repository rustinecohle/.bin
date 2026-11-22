package javaStruct.oop;

public class VehicleRegistry {
    // returns the position of the plate, or -1 if not found
    static int findIndex(Vehicle[] registry, int currentSize, String plate){
        for(int i=0; i<currentSize; i++){
            if(registry[i].plate.equals(plate)){
                return i;
            }
        }
        return -1;
    }
    // shows every vehicle of this owner, returns how many were found
    static int showByOwner(Vehicle[] registry, int currentSize, String owner){
        int count=0;
        for(int i=0; i<currentSize; i++){
            if(registry[i].owner.equals(owner)){
                registry[i].display();
                count++;
            }
        }
        return count;
    }
    // shows every vehicle with expired tax, returns how many were found
    static int showTaxExpired(Vehicle[] registry, int currentSize, int year){
        int count=0;
        for(int i=0; i<currentSize; i++){
            if(registry[i].isTaxExpired(year)){
                registry[i].display();
                count++;
            }
        }
        return count;
    }
    // removes the vehicle by shifting the rest left
    // main must do currentSize-- when this returns true
    static boolean deregister(Vehicle[] registry, int currentSize, String plate){
        int ind=findIndex(registry, currentSize, plate);
        if(ind==-1){
            return false;
        }
        for(int i=ind; i<currentSize-1; i++){
            registry[i]=registry[i+1];
        }
        registry[currentSize-1]=null;
        return true;
    }
}
