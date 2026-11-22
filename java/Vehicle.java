package javaStruct.oop;

public class Vehicle {
    public String plate;
    public String owner;
    public String type;
    public int regYear;
    public int cc;
    public int taxYear;
    public Vehicle(String plate, String owner, String type, int regYear, int cc, int taxYear){
        this.plate=plate;
        this.owner=owner;
        this.type=type;
        this.regYear=regYear;
        this.cc=cc;
        this.taxYear=taxYear;
    }
    public void display(){
        System.out.println("======================");
        System.out.println("plate is "+plate);
        System.out.println("owner is "+owner);
        System.out.println("type is "+type);
        System.out.println("registration year is "+regYear);
        System.out.println("cc is "+cc);
        System.out.println("tax year is "+taxYear);
        System.out.println("======================");
        
    }
    public void transferTo(String newOwner){
        owner=newOwner;
    }
    public boolean isTaxExpired(int currentYear){
        return (currentYear-taxYear)>=2;
    }
}
