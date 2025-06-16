#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <conio.h>

HANDLE hSerial1, hSerial2;

void configurePort(HANDLE hSerial) {
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    GetCommState(hSerial, &dcbSerialParams);

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    dcbSerialParams.fBinary = TRUE;
    dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

    SetCommState(hSerial, &dcbSerialParams);

    COMMTIMEOUTS timeout = {0};
    timeout.ReadIntervalTimeout = 100;
    timeout.ReadTotalTimeoutConstant = 100;
    timeout.ReadTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeout);

    PurgeComm(hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
}

DWORD WINAPI ReadPort(LPVOID lpParameter) {
    HANDLE hSerial = *(HANDLE *)lpParameter;
    const char* portName = (hSerial == hSerial1) ? "COM4" : "COM5";
    char buffer[256];
    DWORD bytesRead, totalBytes = 0;

    printf("Reader thread started for %s\n", portName);

    while (1) {
        for (int i = 0; i < 3; i++) {
            if (ReadFile(hSerial, buffer + totalBytes, sizeof(buffer) - 1 - totalBytes, &bytesRead, NULL)) {
                if (bytesRead > 0) {
                    totalBytes += bytesRead;
                    printf("Read %d bytes on %s\n", bytesRead, portName); // Log bytes read
                    if (buffer[totalBytes - 1] == '\n' || totalBytes >= sizeof(buffer) - 1) {
                        buffer[totalBytes] = '\0';
                        printf("Received on %s: %s", portName, buffer);
                        fflush(stdout);
                        totalBytes = 0;
                        break;
                    }
                } else {
                    break;
                }
            } else {
                DWORD error = GetLastError();
                if (error != ERROR_IO_PENDING) {
                    printf("Read error on %s, error code: %d\n", portName, error);
                    fflush(stdout);
                }
                Sleep(20);
            }
        }
        Sleep(100);
    }

}

int main() {
    printf("Opening COM4 and COM5...\n");

    hSerial1 = CreateFile("\\\\.\\COM4", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    hSerial2 = CreateFile("\\\\.\\COM5", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (hSerial1 == INVALID_HANDLE_VALUE || hSerial2 == INVALID_HANDLE_VALUE) {
        printf("Error opening ports. Error code: %d\n", GetLastError());
        return 1;
    }

    printf("Ports opened successfully!\n");

    configurePort(hSerial1);
    configurePort(hSerial2);

    Sleep(200);

    HANDLE hThread1 = CreateThread(NULL, 0, ReadPort, &hSerial1, 0, NULL);
    HANDLE hThread2 = CreateThread(NULL, 0, ReadPort, &hSerial2, 0, NULL);
    if (hThread1 == NULL || hThread2 == NULL) {
        printf("Error creating threads. Error code: %d\n", GetLastError());
        return 1;
    }

    Sleep(300);

    char msg1[] = "Hello from COM4!\n";
    char msg2[] = "Hello from COM5!\n";
    DWORD bytesWritten;

    printf("Communication started. Press 'q' to quit...\n");
    printf("Sending messages every 2 seconds...\n\n");

    int messageCount = 0;
    while (1) {
        printf("Sending message %d from COM4...\n", ++messageCount);
        if (!WriteFile(hSerial1, msg1, strlen(msg1), &bytesWritten, NULL)) {
            printf("Write error on COM4, error code: %d\n", GetLastError());
        }
        fflush(stdout);
        Sleep(2000);

        printf("Sending message %d from COM5...\n", messageCount);
        if (!WriteFile(hSerial2, msg2, strlen(msg2), &bytesWritten, NULL)) {
            printf("Write error on COM5, error code: %d\n", GetLastError());
        }
        fflush(stdout);
        Sleep(2000);

        if (_kbhit() && _getch() == 'q') {
            printf("\nExiting...\n");
            break;
        }
    }

    CloseHandle(hSerial1);
    CloseHandle(hSerial2);
    return 0;
}