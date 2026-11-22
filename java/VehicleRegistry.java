package javaStruct.oop;

public class VehicleRegistry {
    private Vehicle[] vehicles;
    private int currentSize;

    public VehicleRegistry(int maxSize) {
        vehicles = new Vehicle[maxSize];
        currentSize = 0;
    }

    public boolean isFull() {
        return currentSize >= vehicles.length;
    }

    public int getSize() {
        return currentSize;
    }

    // returns index of the plate, or -1 if not found
    private int indexOf(String plate) {
        for (int i = 0; i < currentSize; i++) {
            if (vehicles[i].getPlate().equals(plate)) {
                return i;
            }
        }
        return -1;
    }

    // returns false if full or duplicate plate
    public boolean add(Vehicle v) {
        if (isFull() || indexOf(v.getPlate()) != -1) {
            return false;
        }
        vehicles[currentSize++] = v;
        return true;
    }

    // returns the vehicle, or null if not found
    public Vehicle findByPlate(String plate) {
        int ind = indexOf(plate);
        if (ind == -1) {
            return null;
        }
        return vehicles[ind];
    }

    public int showByOwner(String owner) {
        int count = 0;
        for (int i = 0; i < currentSize; i++) {
            if (vehicles[i].getOwner().equals(owner)) {
                vehicles[i].display();
                count++;
            }
        }
        return count;
    }

    public void displayAll() {
        for (int i = 0; i < currentSize; i++) {
            vehicles[i].display();
        }
    }

    public int showTaxExpired(int year) {
        int count = 0;
        for (int i = 0; i < currentSize; i++) {
            if (vehicles[i].isTaxExpired(year)) {
                vehicles[i].display();
                count++;
            }
        }
        return count;
    }

    public boolean transfer(String plate, String newOwner) {
        Vehicle v = findByPlate(plate);
        if (v == null) {
            return false;
        }
        v.transferTo(newOwner);
        return true;
    }

    public boolean deregister(String plate) {
        int ind = indexOf(plate);
        if (ind == -1) {
            return false;
        }
        for (int i = ind; i < currentSize - 1; i++) {
            vehicles[i] = vehicles[i + 1];
        }
        vehicles[currentSize - 1] = null;
        currentSize--;          // the size is a field, so the method can really update it
        return true;
    }
}