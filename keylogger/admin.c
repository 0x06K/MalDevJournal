#include <windows.h>
#include <shellapi.h>

void RelaunchAsAdmin() {
    char exePath[MAX_PATH];
    GetModuleFileName(NULL, exePath, MAX_PATH); // Get the current executable path

    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.lpVerb = "runas";      // Request "Run as Administrator"
    sei.lpFile = exePath;      // Path to the executable
    sei.nShow = SW_SHOWNORMAL; // Show the window normally

    if (!ShellExecuteEx(&sei)) {
        MessageBox(NULL, "Failed to elevate privileges!", "Error", MB_OK | MB_ICONERROR);
    }
}

int main() {
    // Check if running as admin
    BOOL isAdmin = FALSE;
    HANDLE token = NULL;

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
        TOKEN_ELEVATION elevation;
        DWORD size;
        if (GetTokenInformation(token, TokenElevation, &elevation, sizeof(elevation), &size)) {
            isAdmin = elevation.TokenIsElevated;
        }
        CloseHandle(token);
    }

    if (!isAdmin) {
        RelaunchAsAdmin();
        return 0; // Exit current instance; elevated one will take over
    }

    // Continue with normal execution
    MessageBox(NULL, "Running as Administrator!", "Success", MB_OK);
    return 0;
}
