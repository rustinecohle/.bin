package javaStruct.fullOop;

public class AccountOperation {
    public static boolean duplicate(Account[] registry, int currentSize,Account temp ){
        for(int i=0; i<currentSize; i++){
            if (registry[i].getAccNo().equals(temp.getAccNo())) {
                return true;
            }
        }return false;
    }

}
