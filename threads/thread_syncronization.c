#include <windows.h>
#include <stdio.h>

#define NUM_THREADS 2

// Shared counter variable
int counter = 0;

// Mutex for synchronization
HANDLE hMutex;

// Thread function to increment the counter
DWORD WINAPI IncrementCounter(LPVOID lpParam) {
    for (int i = 0; i < 100000; i++) {
        // Wait for the mutex to be available
        WaitForSingleObject(hMutex, INFINITE);
        
        counter++;
        
        // Release the mutex after accessing the shared counter
        ReleaseMutex(hMutex);
    }
    return 0;
}

int main() {
    HANDLE threads[NUM_THREADS];
    // Create the mutex (initially not locked)
    hMutex = CreateMutex(NULL, FALSE, NULL);
    
    if (hMutex == NULL) {
        printf("CreateMutex failed: %d\n", GetLastError());
        return 1;
    }

    // Create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        threads[i] = CreateThread(
            NULL, 0, IncrementCounter, NULL, 0, NULL
        );
        if (threads[i] == NULL) {
            printf("CreateThread failed: %d\n", GetLastError());
            return 1;
        }
    }

    // Wait for all threads to finish
    WaitForMultipleObjects(NUM_THREADS, threads, TRUE, INFINITE);

    // Close the threads and mutex handles
    for (int i = 0; i < NUM_THREADS; i++) {
        CloseHandle(threads[i]);
    }
    CloseHandle(hMutex);

    // Print the final counter value
    printf("Counter: %d\n", counter);

    return 0;
}
