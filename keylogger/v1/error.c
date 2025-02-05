#include <windows.h>

int main() {
    // The file path to display in the titl
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, MAX_PATH);

    // The error message
    const char *errorMessage = "Operation did not complete successfully because the file contains a virus or potentially unwanted software.";

    // Display the message box
    MessageBox(
        NULL, // No owner window
        errorMessage, // Message body
        path, // Title of the message box
        MB_ICONERROR | MB_OK // Error icon and OK button
    );

    return 0;
}
