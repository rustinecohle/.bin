package javaStruct.contact;
import java.io.*;
import java.util.Scanner;

public class ContactOperation {
    static boolean isValidPhone(String s){
        boolean ok=true;
        if(s.length()!=11 || !s.startsWith("01")){
            ok=false;
        }
        for(int i=0; i<s.length(); i++){
            if(!Character.isDigit(s.charAt(i))){
                ok=false;
            }
        }
        if(!ok){
            System.out.println("invalid must start with 01 and have 11 digits");
        }
        return ok;
    }
    static boolean isValidEmail(String s){
        int count=0;
        boolean space=false;
        for(int i=0; i<s.length(); i++){
            if(s.charAt(i)=='@'){
                count++;
            }
            if(s.charAt(i)==' '){
                space=true;
            }
        }
        if(count==1 && !space){
            return true;
        }
        System.out.println("invalid must have one @ and no spaces");
        return false;
    }
    static boolean duplicate(Contact temp, int size, Contact[] registry){
        for(int i=0; i<size; i++){
            if(registry[i].no.equals(temp.no)){
                return true;
            }
        }
        return false;
    }
    static void addNew(Contact temp, Scanner sc){
        System.out.println("enter name");
        temp.name=sc.next();
        do{
            System.out.println("enter number (starts with 01, 11 digits)");
            temp.no=sc.next();
        }while(!isValidPhone(temp.no));
        do{
            System.out.println("enter email");
            temp.email=sc.next();
        }while(!isValidEmail(temp.email));
        System.out.println("enter group");
        temp.group=sc.next();
        do{
            System.out.println("enter birth year (1900 to 2100)");
            temp.year=sc.nextInt();
        }while(temp.year<1900 || temp.year>2100);
    }
    static void display(Contact c){
        System.out.println("\n=========================");
        System.out.println("name "+c.name);
        System.out.println("number "+c.no);
        System.out.println("email "+c.email);
        System.out.println("group "+c.group);
        System.out.println("year "+c.year);
        System.out.println("=========================");
    }
    // fills result with the matching contacts (like Contact *result[] in C)
    static int searchByName(Contact[] registry, int size, Contact[] result, String name){
        int count=0;
        for(int i=0; i<size && count<result.length; i++){
            if(registry[i].name.equals(name)){
                result[count]=registry[i];
                count++;
            }
        }
        return count;
    }
    static int listByGroup(Contact[] registry, int size, String group){
        int count=0;
        for(int i=0; i<size; i++){
            if(registry[i].group.equals(group)){
                display(registry[i]);
                count++;
            }
        }
        return count;
    }
    static int saveToFile(Contact[] registry, int size, String filename){
        try{
            PrintWriter out=new PrintWriter(new FileWriter(filename));
            int count=0;
            for(int i=0; i<size; i++){
                out.println(registry[i].name+"|"+registry[i].no+"|"+registry[i].email+"|"+registry[i].group+"|"+registry[i].year);
                count++;
            }
            out.close();
            return count;
        }catch(IOException e){
            System.out.println("cannot open file");
            return -1;
        }
    }
    static int loadFromFile(Contact[] registry, int maxSize, String filename){
        try{
            Scanner file=new Scanner(new File(filename));
            int count=0;
            while(file.hasNextLine() && count<maxSize){
                String line=file.nextLine();
                String[] parts=line.split("\\|");
                if(parts.length==5){
                    try{
                        Contact c=new Contact();
                        c.name=parts[0];
                        c.no=parts[1];
                        c.email=parts[2];
                        c.group=parts[3];
                        c.year=Integer.parseInt(parts[4].trim());
                        registry[count]=c;
                        count++;
                    }catch(NumberFormatException e){
                        System.out.println("warning skipping bad lines");
                    }
                }
                else System.out.println("warning skipping bad lines");
            }
            file.close();
            return count;
        }catch(FileNotFoundException e){
            System.out.println("cannot open file");
            return -1;
        }
    }
}
