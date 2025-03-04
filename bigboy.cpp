#include <iostream>
using namespace std;
void insertionSort(int arr[], int n){
  for(int i = 1; i < n; i++){
    for(int j = i; j > i-1; j--){
      if(arr[j] < arr[j-1]){
        arr[j+1] = arr[j];
        arr[j] = arr[j-1];
        arr[j-1] = arr[j];

  }
  else{
    break;
  }

  }//
  }
  }
  int main(){
    int n;
    int arr[10] = {12,0,3,4,90,6,80,8,9};
    insertionSort(arr,10);
    for(int i = 0; i < 10; i++){
      cout << arr[i] << " ";

  }
  cout << endl;
  return 0;
  }
// Created by DELL on 2/27/2025.
//
