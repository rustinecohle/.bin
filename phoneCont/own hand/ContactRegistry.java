package javaStruct.fullOop;
import java.util.Scanner;
public class ContactRegistry {
    static Scanner sc=new Scanner(System.in);
    public static  void  addNew(Contact temp){
        System.out.println("enetr name ");
        temp.name=sc.next();
        System.out.println("enter number ");
        temp.no=sc.next();
        System.out.println("enetr email ");
        temp.email=sc.next();
        System.out.println("enter group ");
        temp.group=sc.next();
        System.out.println("enter year");
        temp.year=sc.nextInt();
    }
    public static void display(Contact temp){
        System.out.println("\n=========================" );
        System.out.println("name is "+temp.name);
        System.out.println("number is "+temp.no);
        System.out.println("email is "+temp.email);
        System.out.println("group is "+temp.group);
        System.out.println("year is "+temp.year);
    }
    public static void sort(Contact[] registry, int currentSize)
    {
        for(int i=0; i<currentSize-1; i++){
            for(int j=i+1;j<currentSize-i-1;j++) {
                // if(registry[j].name<registry[j+1].name)
                if(registry[j].name.compareToIgnoreCase(registry[j+1].name)<0){
                    String temp=registry[j].name;
                    registry[j].name=registry[j+1].name;
                    registry[j+1].name=temp;

                }
            }
        }
    }
    public static int index(Contact[] registry, int currentSize, String name){
        for(int i=0; i<currentSize;i++){
            if(registry[i].name.equals(name)){
                return i;
            }
        }
        return -1;
    }
    public static boolean delete(Contact[] registry, int currentSize, String name){
        int ind=index(registry, currentSize, name);
        if(ind==-1){
            System.out.println("could not find name");
            return false;
        }
        for(int i=ind;i<currentSize-1;i++){
            registry[i]=registry[i+1];
        }
        return true;

    }


}

