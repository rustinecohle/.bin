package javaStruct.contactOop;
import java.io.*;
import java.util.Scanner;

public class ContactMain {
    static Scanner sc=new Scanner(System.in);

    static boolean saveToFile(Contact person, String filename){
        try{
            PrintWriter out=new PrintWriter(new FileWriter(filename));
            out.println(person.toFileLine());
            out.close();
            return true;
        }catch(IOException e){
            return false;
        }
    }
    // returns null if the file is missing or the line is bad
    static Contact loadFromFile(String filename){
        try{
            Scanner file=new Scanner(new File(filename));
            Contact result=null;
            if(file.hasNextLine()){
                result=Contact.fromFileLine(file.nextLine());
            }
            file.close();
            return result;
        }catch(FileNotFoundException e){
            return null;
        }
    }

    public static void main(String[] args) {
        Contact person=null;
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
                    if(person!=null){
                        System.out.println("a contact is already saved!");
                        break;
                    }
                    System.out.println("enter name");
                    String name=sc.next();

                    String no;
                    do{
                        System.out.println("enter number (starts with 01, 11 digits)");
                        no=sc.next();
                    }while(!Contact.isValidPhone(no));

                    String email;
                    do{
                        System.out.println("enter email (one @, no spaces)");
                        email=sc.next();
                    }while(!Contact.isValidEmail(email));

                    System.out.println("enter group");
                    String group=sc.next();

                    int year;
                    do{
                        System.out.println("enter birth year (1900 to 2100)");
                        year=sc.nextInt();
                    }while(!Contact.isValidYear(year));

                    person=new Contact(name, no, email, group, year);
                    System.out.println("contact added!");
                    break;
                }
                case 2:{
                    if(person==null){
                        System.out.println("no contact saved");
                        break;
                    }
                    System.out.println("enter name");
                    String name=sc.next();
                    if(person.hasName(name)){
                        person.display();
                    }
                    else System.out.println("no results found");
                    break;
                }
                case 3:{
                    if(person==null){
                        System.out.println("no contact saved");
                        break;
                    }
                    System.out.println("enter group");
                    String group=sc.next();
                    if(person.isInGroup(group)){
                        person.display();
                        System.out.println("found 1");
                    }
                    else System.out.println("no results found");
                    break;
                }
                case 4:{
                    if(person==null){
                        System.out.println("enter data first");
                        break;
                    }
                    person.display();
                    break;
                }
                case 5:{
                    if(person==null){
                        System.out.println("nothing to save");
                        break;
                    }
                    if(saveToFile(person, "saved.txt")){
                        System.out.println("wrote 1 lines");
                    }
                    else System.out.println("failed");
                    break;
                }
                case 6:{
                    Contact loaded=loadFromFile("givenfile.txt");
                    if(loaded==null){
                        System.out.println("failed (file missing or bad line)");
                    }
                    else{
                        person=loaded;
                        System.out.println("loaded 1 lines");
                    }
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
