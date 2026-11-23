package javaStruct.fullOop;
import java.io.PrintWriter;
import java.io.FileWriter;
import java.io.IOException;
public class StudentOperation {
    public static boolean duplicate(Student[] registry,int roll, String name,int currentSize){
        for(int i=0; i<currentSize; i++){
            if(registry[i].getRoll()==roll || registry[i].getName().equals(name)){
                return true;
            }
            
        }   
        return false;
    }
    public static int topper(Student[] registry, int currentSize ){
        float max=0;
        int maxind=0;
        for(int i=0; i<currentSize; i++){
            if(registry[i].getMark()>max){
                max=registry[i].getMark();
                maxind=i;
            }
        }
        return maxind;
    }
    public static int saveToFile(Student[] registry, int currentSize, String filename){
        int count=0;
        try(PrintWriter out=new PrintWriter(new FileWriter(filename))){
            for(int i=0; i<currentSize; i++){
                out.println(registry[i].toFile());
                count++;
            }
            if(out.checkError()){
                return -1;
            }
            return count;
        }catch(IOException e){
            return -1;
        }
    }


}
