#include <stdio.h>
#include <windows.h>


DWORD WINAPI PrintNumbers(LPVOID lpParam) {
    while(1){for (int i = 1; i <= 5; i++) {
        printf(" %d ", i);
        fflush(stdout);
        Sleep(100);  // Sleep for 1 second between prints
    }}
    return 0;
}

int main() { 
    HANDLE threadHandle;
    DWORD threadId;
    threadHandle = CreateThread(
        NULL,
        0,
        PrintNumbers,
        NULL,
        0,
        &threadId
    );
    
    if (threadHandle == NULL) {
        printf("Error creating thread: %d\n", GetLastError());
        return 1;
    }
    
    while(1){   
        for (int i = 1; i <= 5; i++) {
            printf("%d \n", i);
            Sleep(50);  // Sleep for 1 second between prints
        }
    }
    WaitForSingleObject(threadHandle, INFINITE);
}