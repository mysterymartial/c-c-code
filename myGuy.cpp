

#include <iostream>
void bubbleSort(int arr[]) {
    int len = 0;
    while (arr[len] != -1) {
        len++;

    }
    int swapped = 0;
    for (int count = 0; count < len-1; count++) {
        swapped = 0;
        for (int i = 0; i < len-count-1; i++) {
            if (arr[i] > arr[i+1]) {
                int temp = arr[i];
                arr[i] = arr[i+1];
                arr[i+1] = temp;
                swapped = 1;
            }
        }
    }
}


int main(){

    int arr[] = {8, 10, 3, 4, 1, 5, 7, 2, 9, 50, -1};
    //int lastIndex = sizeof(arr)-1;
    bubbleSort(arr);
    int len = 0;

    while (arr[len] != -1) {
        std::cout << arr[len] << " ";
        len++;

    }
    std::cout << std::endl;
    return 0;
}