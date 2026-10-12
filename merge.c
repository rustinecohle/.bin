#include <stdio.h>

void final_merge(int arr[], int start, int mid, int end);

void merge_sort(int arr[], int start, int end);


int main(){
    
    int arr[] = {23, 43, 53, 74, 83, 99, 234, -3, -345};

    int size = sizeof(arr) / sizeof (arr[0]);

    merge_sort(arr, 0, size - 1);

    for (int i = 0; i < size; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");


    return 0;
}

void final_merge(int arr[], int start, int mid, int end){
    int temp[500];

    int i = start; 
    int j = mid + 1; 

    int temp_index = 0; 

    while (i <= mid && j <= end)
    {
        if (arr[i] <= arr[j])   
        {
            temp[temp_index] = arr[i];
            i++;
        }
        else
        {
            temp[temp_index] = arr[j];
            j++;
        }

        temp_index++;   
    }

    while (i <= mid)
    {
        temp[temp_index] = arr[i];
        i++;
        temp_index++;
    }

    while (j <= end)
    {
        temp[temp_index] = arr[j];
        j++;
        temp_index++; 
    }

    for (int i = 0; i < temp_index; i++)
    {
        arr[start + i] = temp[i]; 
    }
    
    
    
    
}

void merge_sort(int arr[], int start, int end){

    if (start < end)
    {
        int mid = start + (end - start) / 2;

        merge_sort(arr, start, mid);

        merge_sort(arr, mid + 1, end);

        final_merge(arr, start, mid, end);
    }
    
}
