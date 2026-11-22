package javaStruct.contactOop;

public class Contact {
    private String name;
    private String no;
    private String email;
    private String group;
    private int year;

    // the rules live inside the class
    public static boolean isValidPhone(String s){
        if(s.length()!=11 || !s.startsWith("01")){
            return false;
        }
        for(int i=0; i<s.length(); i++){
            if(!Character.isDigit(s.charAt(i))){
                return false;
            }
        }
        return true;
    }
    public static boolean isValidEmail(String s){
        int count=0;
        for(int i=0; i<s.length(); i++){
            if(s.charAt(i)=='@'){
                count++;
            }
            if(s.charAt(i)==' '){
                return false;
            }
        }
        return count==1;
    }
    public static boolean isValidYear(int year){
        return year>=1900 && year<=2100;
    }

    // the constructor refuses bad data
    public Contact(String name, String no, String email, String group, int year){
        if(name.isEmpty() || group.isEmpty()){
            throw new IllegalArgumentException("name and group cannot be empty");
        }
        if(!isValidPhone(no)){
            throw new IllegalArgumentException("phone must start with 01 and have 11 digits");
        }
        if(!isValidEmail(email)){
            throw new IllegalArgumentException("email must have one @ and no spaces");
        }
        if(!isValidYear(year)){
            throw new IllegalArgumentException("year must be 1900 to 2100");
        }
        this.name=name;
        this.no=no;
        this.email=email;
        this.group=group;
        this.year=year;
    }

    // the contact answers questions about itself
    public boolean hasName(String n){
        return name.equals(n);
    }
    public boolean isInGroup(String g){
        return group.equals(g);
    }

    public void display(){
        System.out.println("\n=========================");
        System.out.println("name "+name);
        System.out.println("number "+no);
        System.out.println("email "+email);
        System.out.println("group "+group);
        System.out.println("year "+year);
        System.out.println("=========================");
    }

    // used for saving: one line of text
    public String toFileLine(){
        return name+"|"+no+"|"+email+"|"+group+"|"+year;
    }

    // used for loading: builds a Contact from one line, or returns null if the line is bad
    public static Contact fromFileLine(String line){
        String[] parts=line.split("\\|");
        if(parts.length!=5){
            return null;
        }
        try{
            int year=Integer.parseInt(parts[4].trim());
            return new Contact(parts[0], parts[1], parts[2], parts[3], year);
        }catch(IllegalArgumentException e){
            // catches a bad year number and also the constructor's "throw"
            return null;
        }
    }
}
