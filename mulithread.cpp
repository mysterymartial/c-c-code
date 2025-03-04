//
// Created by DELL on 3/3/2025.
//
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* printTask(void* arg){

  for(int i = 0; i < 10; i++){
    printf("%d\n", i);
    sleep(1);
  }
  return NULL;
}

int main(){
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, printTask, NULL);
  pthread_create(&thread2, NULL, printTask, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  printf("main thread exit\n");
  return 0;
}