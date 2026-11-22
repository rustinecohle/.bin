package javaStruct.contact;
import java.util.Scanner;

public class ContactMain {
    static final int MAX=200;
    static Scanner sc=new Scanner(System.in);
    public static void main(String[] args) {
        int size=0;
        Contact[] phone=new Contact[MAX];
        int c;
        do{
            System.out.println("\n================PHONE CONTACT================");
            System.out.println("1. add contact");
            System.out.println("2. Search name");
            System.out.println("3. list by group");
            System.out.println("4. display all");
            System.out.println("5. save to file");
            System.out.println("6. load from file");
            System.out.println("0. exit");
            System.out.println("enter choice");
            c=sc.nextInt();
            switch(c){
                case 1:{
                    if(size>=MAX){
                        System.out.println("max size reached");
                        break;
                    }
                    Contact temp=new Contact();
                    ContactOperation.addNew(temp, sc);
                    if(ContactOperation.duplicate(temp, size, phone)){
                        System.out.println("duplicate found");
                        break;
                    }
                    phone[size]=temp;
                    size++;
                    break;
                }
                case 2:{
                    Contact[] result=new Contact[MAX];
                    System.out.println("enter name");
                    String name=sc.next();
                    int value=ContactOperation.searchByName(phone, size, result, name);
                    if(value>0){
                        for(int i=0; i<value; i++){
                            ContactOperation.display(result[i]);
                        }
                    }
                    else System.out.println("no results found");
                    break;
                }
                case 3:{
                    System.out.println("enter group");
                    String group=sc.next();
                    int value=ContactOperation.listByGroup(phone, size, group);
                    if(value>0){
                        System.out.println("found "+value);
                    }
                    else System.out.println("no results found");
                    break;
                }
                case 4:{
                    for(int i=0; i<size; i++){
                        ContactOperation.display(phone[i]);
                    }
                    break;
                }
                case 5:{
                    int value=ContactOperation.saveToFile(phone, size, "saved.txt");
                    if(value>-1){
                        System.out.println("wrote "+value+" lines");
                    }
                    else System.out.println("failed");
                    break;
                }
                case 6:{
                    int value=ContactOperation.loadFromFile(phone, MAX, "givenfile.txt");
                    if(value>-1){
                        System.out.println("loaded "+value+" lines");
                        size=value;
                    }
                    else System.out.println("failed");
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
