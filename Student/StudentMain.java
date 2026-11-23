package javaStruct.fullOop;

import java.util.Scanner;


public class StudentMain {
    static Scanner sc=new Scanner(System.in);
    static final int MAX=150;
    public static void main(String[] args) {
        Student[] registry=new Student[MAX];
        int currentSize=0;
        int c;
        do{
            System.out.println("\n============= RESULT MANAGEMENT =============");
            System.out.println("1. Add new result");
            System.out.println("2. Search result by roll");
            System.out.println("3. Display all");
            System.out.println("4. Failed student list");
            System.out.println("5. Course topper");
            System.out.println("6. Save to file");
            System.out.println("0. Exit");

            System.out.print("Enter choice: ");
            c = sc.nextInt();
            switch (c) {
                case 1:{
                    if(currentSize>=MAX){
                        System.out.println("maxxxxxxxxed");
                        break;
                    }
                    System.out.print("Enter roll: ");
                    int roll = sc.nextInt();
                    sc.nextLine();

                    System.out.print("Enter name: ");
                    String name = sc.nextLine();

                    System.out.print("Enter code: ");
                    String code = sc.next();

                    System.out.print("Enter mark: ");
                    float mark = sc.nextFloat();

                    System.out.print("Enter semester: ");
                    int semester = sc.nextInt();
                    boolean value=StudentOperation.duplicate(registry,roll, name,currentSize);
                    if(value){
                        System.out.println("duplicate found");
                    }
                    else{
                        registry[currentSize]=new Student(roll, name, code
                
                    ,mark, semester);
                        currentSize++;
                    }
                    break;
                }
                case 2:{
                    System.out.println("emnter roll ");
                    int roll=sc.nextInt();
                    for(int i=0; i<currentSize; i++){
                        if(registry[i].getRoll()==roll){
                            registry[i].display();
                        }
                    }break;
                }
                case 3:{
                    if(currentSize==0){
                        System.out.println("empty");break;
                    }
                    for(int i=0; i<currentSize; i++){
                        registry[i].display();
                    }
                    break;
                }
                case 4:{
                    System.out.println("enter pass mark ");
                    float pass=sc.nextFloat();
                    for(int i=0; i<currentSize;i++){
                        if((pass-registry[i].getMark())<0){
                            registry[i].display();
                        }
                    }break;

                }
                case 5:{
                    int value=StudentOperation.topper(registry, currentSize);
                    registry[value].display();
                    break;
                }
                case 6:{
                    // System.out.println("en");
                    int value=StudentOperation.saveToFile(registry, currentSize, "saved.txt");
                    if(value==-1){
                        System.out.println("error");
                    }else System.out.println("saved lines"+value);
                    break;
                }
            }
        }while(c!=0);
        sc.close();
    }
}
