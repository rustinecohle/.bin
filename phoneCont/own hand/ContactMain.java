package javaStruct.fullOop;
import java.util.Scanner;
public class ContactMain {
    static Scanner sc=new Scanner(System.in);
    public static void main(String[] args) {
        Contact[] registry=new Contact[10];
        int currentSize=0;
        int c;
        do{
            System.out.println("\n=================");
            System.out.println("1.add contact");
            System.out.println("2.display contact");
            System.out.println("3.search contact");
            System.out.println("4.sort by names");
            System.out.println("6.delete contact");
            System.out.println("10.search and sort (special)");
            System.out.println("enetr choice ");
            c=sc.nextInt();
            switch(c){
                case 1:{
                    if(currentSize>=150){
                        System.out.println("registry is full");
                        break;
                    }
                    Contact temp=new Contact();
                    ContactRegistry.addNew(temp);
                    boolean duplicate=false;
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].name.equals(temp.name)){
                            duplicate=true;
                        }
                    }
                    if(duplicate){
                        System.out.println("duplicate found");

                    }
                    else{
                        registry[currentSize]=temp;
                        currentSize++;
                        System.out.println("added successfully");
                }
                    break;

                }
                case 2:{
                    for(int i=0; i<currentSize; i++){
                        ContactRegistry.display(registry[i]);
                    }
                    break;
            }
                case 3:{
                    System.out.println("enter name ");
                    String name=sc.next();
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].name.equals(name)){
                            ContactRegistry.display(registry[i]);
                        }
                    } break;

                }
                case 4:{
                    ContactRegistry.sort(registry, currentSize );
                    for(int i=0; i<currentSize;i++){
                        ContactRegistry.display(registry[i]);
                    }
                    break;
                }
                case 5:
                    {
                        System.out.println("enter name to search");
                        String name=sc.next();
                        boolean found=ContactRegistry.delete(registry, currentSize, name);
                        if(found){
                            System.out.println("deleted successfully!!!!");
                            currentSize--;
                        }
                        else {
                            System.out.println("could not delete");

                        }
                        break;
                    }

                case 10:{
                    System.out.println("enter to search");
                    String name=sc.next();
                    int count=0;
                    for(int i=0; i<currentSize;i++){
                        if(registry[i].name.contains(name)){
                            ContactRegistry.display(registry[i]);
                            count++;
                        }
                    }
                    System.out.println("found "+count+"results with keyword"+name);
                }

        }
        }while(c!=0);
        sc.close();
    }
}
