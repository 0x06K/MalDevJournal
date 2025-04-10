#include <windows.h>
#include <stdio.h>
#include <Userenv.h>
#include <winbase.h>
// this technique is not process hollowing
BOOL CreateSystemProcess() {
    HANDLE hToken;
    HANDLE hDupToken;
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);

    // Get the access token for the current process
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE | TOKEN_QUERY, &hToken)) {
        printf("Failed to open process token.\n");
        return FALSE;
    }

    // Duplicate the token to impersonate SYSTEM
    if (!DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL, SecurityImpersonation, TokenPrimary, &hDupToken)) {
        printf("Failed to duplicate the token.\n");
        CloseHandle(hToken);
        return FALSE;
    }

    // Now, create the process with the SYSTEM token
    if (!CreateProcessWithTokenW(hDupToken, LOGON_WITH_PROFILE, NULL, L"cmd.exe", 0, NULL, NULL, &si, &pi)) {
        printf("Failed to create process with SYSTEM token.\n");
        CloseHandle(hToken);
        CloseHandle(hDupToken);
        return FALSE;
    }

    printf("Created cmd.exe with SYSTEM privileges.\n");

    // Wait for the process to terminate
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Cleanup
    CloseHandle(hToken);
    CloseHandle(hDupToken);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return TRUE;
}

int main() {
    if (!CreateSystemProcess()) {
        printf("Failed to create SYSTEM process.\n");
        return 1;
    }
    return 0;
}
