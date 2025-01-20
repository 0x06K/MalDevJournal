#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define SERVER_IP "192.168.1.14"  // Change this to the Linux server IP
#define SERVER_PORT 8080
#define BUF_SIZE 1024

// Function to send keystrokes to the server
void send_keystroke(SOCKET sock, const char* key) {
    send(sock, key, strlen(key), 0);
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
    char name[7];
    fgets(name, sizeof(name),stdin);
    if(send(sock, name, strlen(name), 0) < 0) {
        printf("Error sending name\n");
        return 1;
    } else {
        printf("Name sent successfully\n");
    }
    
    // Start capturing keystrokes
    while (1) {
        char message[1024];
        fgets(message, 1024, stdin);
        if(send(sock, message, strlen(message), 0) < 0) {
            printf("Error sending message\n");
            return 1;
        }
    }

    // Cleanup
    closesocket(sock);
    WSACleanup();
    return 0;
}
