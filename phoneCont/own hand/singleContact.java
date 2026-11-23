package javaStruct.fullOop;
import java.util.Scanner;
public class singleContact {
    private String name;
    private String no;
    private String email;
    private String group;
    private int year;
    public singleContact(String name, String no, String email, String group, int year){
        this.name=name;
        this.no=no;
        this.email=email;
        this.group=group;
        this.year=year;
    }
    public void display(){
        System.out.println("\n=======================");
        System.out.println("name is "+name);
        System.out.println("nmber is "+no);
        System.out.println("email is "+email);
        System.out.println("group is "+group);
        System.out.println("year is "+year);
    }
    

}
