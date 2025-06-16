#include <windows.h>
#include <stdio.h>
#include <string.h>

HANDLE hSerial1, hSerial2;

void configurePort(HANDLE hSerial){
  DCB dcbSerialParams = {0};
  dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
  GetCommState(hSerial, &dcbSerialParams);
  dcbSerialParams.BaudRate = CBR_9600;
  dcbSerialParams.ByteSize = 8;
  dcbSerialParams.StopBits = ONESTOPBIT;
  dcbSerialParams.Parity = NOPARITY;
  SetCommState(hSerial, &dcbSerialParams);

  COMMTIMEOUTS timeout = {0};
  timeout.ReadIntervalTimeout = 50;
  timeout.ReadTotalTimeoutConstant = 50;
  timeout.ReadTotalTimeoutMultiplier = 50;
  timeout.WriteTotalTimeoutConstant = 50;
  timeout.WriteTotalTimeoutMultiplier = 50;
  SetCommTimeouts(hSerial, &timeout);

}

DWORD WINAPI ReadPort(LPVOID lpParameter){
  HANDLE hSerial = *(HANDLE *)lpParameter;
  char portName[16];
  sprintf(portName, "%s", hSerial == hSerial1 ? "COM1" : "COM2");

  char buffer[256];
  DWORD bytesRead;
  while (1){
    if(ReadFile(hSerial, buffer, sizeof(buffer) -1, &bytesRead, NULL)){
      if(bytesRead > 0){
        buffer[bytesRead] = '\0';
        printf("Received on %s: %s", portName, buffer);
      }
    }
    Sleep(100);

  }
  return 0;
}

int main(){
  hSerial1 = CreateFile("\\\\.\\COM1", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
  hSerial2 = CreateFile("\\\\.\\COM2", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

  if(hSerial1 == INVALID_HANDLE_VALUE || hSerial2 == INVALID_HANDLE_VALUE){
    printf("Error opening serial port: %d\n", GetLastError());
    return 1;
  }
  configurePort(hSerial1);
  configurePort(hSerial2);

  HANDLE hThread1 = CreateThread(NULL, 0, ReadPort, &hSerial1, 0, NULL);
  HANDLE hThread2 = CreateThread(NULL, 0, ReadPort, &hSerial2, 0, NULL);

  char msg1[] = "Hello from COM1!\n";
  char msg2[] = "Hello from COM2!\n";
  DWORD bytesWritten;

  while (1){
    WriteFile(hSerial1, msg1, strlen(msg1), &bytesWritten, NULL);
    printf("Sent message from COM1: %s", msg1);
    Sleep(1000);

    WriteFile(hSerial2, msg2, strlen(msg2), &bytesWritten, NULL);
    printf("Sent message from COM2: %s", msg2);
    Sleep(1000);

    if(GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState('C')){
      printf("\nClosing ports.....\n");
      break;
    }

  }
  CloseHandle(hThread1);
  CloseHandle(hThread2);
  CloseHandle(hSerial1);
  CloseHandle(hSerial2);
  return 0;
}