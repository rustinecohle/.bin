package javaStruct.fullOop;

public class Student {
    private int roll;
    private String name;
    private String code;
    private float mark;
    private int semester;

    Student(int roll,String name, String code, float mark,int semester){
        this.roll=roll;
        this.name=name;
        this.code=code;
        this.mark=mark;
        this.semester=semester;
    }

    public void display(){
        System.out.println("\n==========================");
        System.out.println("roll is "+roll);
        System.out.println("name is "+name);
        System.out.println("code is "+code);
        System.out.println("mark is "+mark);
        System.out.println("semester is "+semester);

    }
    public int getRoll(){
        return roll;
    }
    public String getName(){
        return name;
    }
    public float getMark(){
        return mark;
    }

    public String toFile(){
        return roll+"|"+name+"|"+code+"|"+mark+"|"+semester;
    }
}
