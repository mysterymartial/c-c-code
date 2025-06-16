#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

int main() {
    printf("Testing virtual COM ports...\n");

    // Use the virtual COM ports created by com0com
    HANDLE hSerial1 = CreateFile("\\\\.\\COM3", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    HANDLE hSerial2 = CreateFile("\\\\.\\COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hSerial1 == INVALID_HANDLE_VALUE || hSerial2 == INVALID_HANDLE_VALUE) {
        printf("Error opening virtual COM ports: %d\n", GetLastError());
        printf("Make sure com0com is installed and COM3-COM4 pair is created\n");
        return 1;
    }

    printf("✓ Successfully opened COM3 and COM4!\n");

    // Configure ports
    DCB dcb = {0};
    dcb.DCBlength = sizeof(dcb);
    GetCommState(hSerial1, &dcb);
    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    SetCommState(hSerial1, &dcb);
    SetCommState(hSerial2, &dcb);

    // Test communication
    char testMsg[] = "Hello from COM3!";
    char buffer[256];
    DWORD bytesWritten, bytesRead;

    printf("Sending message from COM3 to COM4...\n");
    WriteFile(hSerial1, testMsg, strlen(testMsg), &bytesWritten, NULL);

    Sleep(100); // Small delay

    ReadFile(hSerial2, buffer, sizeof(buffer)-1, &bytesRead, NULL);
    if (bytesRead > 0) {
        buffer[bytesRead] = '\0';
        printf("✓ Received on COM4: %s\n", buffer);
        printf("Virtual COM ports are working!\n");
    } else {
        printf("No data received. Check com0com configuration.\n");
    }

    CloseHandle(hSerial1);
    CloseHandle(hSerial2);

    printf("Press Enter to exit...\n");
    getchar();
    return 0;
}
//
// Created by DELL on 6/16/2025.
//
