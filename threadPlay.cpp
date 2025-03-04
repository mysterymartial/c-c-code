//
// Created by DELL on 3/3/2025.
//

#include <iostream>
#include <thread>
#include <chrono>

void printNumber(){
  for(int i=0;i<10;i++){
    std::cout<<i << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(500));
  }
  }

int main(){
  std:: thread thread1(printNumber);
  std:: thread thread2(printNumber);

  thread1.join();
  thread2.join();

  std::cout<<"main thread finished"<<std::endl;
  return 0;
}