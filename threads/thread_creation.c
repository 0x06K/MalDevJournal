#include <stdio.h>
#include <windows.h>

// Thread function for printing numbers
DWORD WINAPI PrintNumbers(LPVOID lpParam) {
    while (1) {
        for (int i = 1; i <= 5; i++) {
            printf(" %d ", i);
            fflush(stdout);
            Sleep(100);  // Sleep for 100ms between prints
        }
    }
    return 0;
}

// Thread function for printing letters
DWORD WINAPI PrintLetters(LPVOID lpParam) {
    while (1) {
        for (char c = 'A'; c <= 'E'; c++) {
            printf("%c\n", c);
            Sleep(50);  // Sleep for 50ms between prints
        }
    }
    return 0;
}

// Thread function for printing symbols
DWORD WINAPI PrintSymbols(LPVOID lpParam) {
    char symbols[] = {'@', '#', '$', '%', '&'};
    while (1) {
        for (int i = 0; i < 5; i++) {
            printf("%c ", symbols[i]);
            fflush(stdout);
            Sleep(75);  // Sleep for 75ms between prints
        }
    }
    return 0;
}

int main() {
    HANDLE numbersThread, alphabetsThread, symbolsThread;
    DWORD numbersThreadId, alphabetsThreadId, symbolsThreadId;

    // Create threads
    numbersThread = CreateThread(NULL, 0, PrintNumbers, NULL, 0, &numbersThreadId);
    alphabetsThread = CreateThread(NULL, 0, PrintLetters, NULL, 0, &alphabetsThreadId);
    symbolsThread = CreateThread(NULL, 0, PrintSymbols, NULL, 0, &symbolsThreadId);

    // Set thread priorities for parallel execution (optional)
    SetThreadPriority(numbersThread, THREAD_PRIORITY_ABOVE_NORMAL);
    SetThreadPriority(alphabetsThread, THREAD_PRIORITY_ABOVE_NORMAL);
    SetThreadPriority(symbolsThread, THREAD_PRIORITY_ABOVE_NORMAL);

    // Error checking
    if (numbersThread == NULL) {
        printf("Error creating numbers thread: %d\n", GetLastError());
        return 1;
    }
    if (alphabetsThread == NULL) {
        printf("Error creating letters thread: %d\n", GetLastError());
        return 1;
    }
    if (symbolsThread == NULL) {
        printf("Error creating symbols thread: %d\n", GetLastError());
        return 1;
    }

    // Wait for all threads to complete (infinite loop, so they never complete)
    WaitForSingleObject(numbersThread, INFINITE);
    WaitForSingleObject(alphabetsThread, INFINITE);
    WaitForSingleObject(symbolsThread, INFINITE);

    return 0;
}
