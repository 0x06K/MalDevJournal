#include <windows.h>
#include <stdio.h>

HANDLE hEvent;  // Event handle for communication
int sharedData = 0;  // Shared resource

// Producer thread function
DWORD WINAPI Producer(LPVOID lpParam) {
    for (int i = 1; i <= 5; i++) {
        // Simulate producing data
        Sleep(1000);
        sharedData = i;

        printf("Producer: Produced data %d\n", sharedData);

        // Signal the consumer thread
        SetEvent(hEvent);
    }
    return 0;
}

// Consumer thread function
DWORD WINAPI Consumer(LPVOID lpParam) {
    for (int i = 1; i <= 5; i++) {
        // Wait for the producer to signal
        WaitForSingleObject(hEvent, INFINITE);

        printf("Consumer: Consumed data %d\n", sharedData);

        // Reset the event for the next iteration
        ResetEvent(hEvent);
    }
    return 0;
}

int main() {
    HANDLE hProducer, hConsumer;

    // Create an auto-reset event
    hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (hEvent == NULL) {
        printf("CreateEvent failed (%d)\n", GetLastError());
        return 1;
    }
    /*
        Event functio signature
        HANDLE CreateEvent(
            LPSECURITY_ATTRIBUTES lpEventAttributes, // Security attributes (usually NULL for defaults)
            BOOL bManualReset,                       // Manual-reset or auto-reset event
            BOOL bInitialState,                      // Initial state: signaled or non-signaled
            LPCSTR lpName                            // Name of the event (optional, can be NULL)
        );

    */

    // Create producer and consumer threads
    hProducer = CreateThread(NULL, 0, Producer, NULL, 0, NULL);
    hConsumer = CreateThread(NULL, 0, Consumer, NULL, 0, NULL);

    // Wait for both threads to finish
    WaitForSingleObject(hProducer, INFINITE);
    WaitForSingleObject(hConsumer, INFINITE);

    // Clean up
    CloseHandle(hProducer);
    CloseHandle(hConsumer);
    CloseHandle(hEvent);

    return 0;
}
