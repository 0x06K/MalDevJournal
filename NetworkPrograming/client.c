#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

int main(){
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in _server;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData)) {
        printf("Initialization failed\n");
        return 1;
    }
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    _server.sin_family = AF_INET;
    _server.sin_port = htons(22);
    _server.sin_addr.s_addr = inet_addr("192.168.1.12");
    if(connect(sock, (SOCKADDR*)&_server, sizeof(_server)) == SOCKET_ERROR) {
        printf("Connection failed\n");
        WSACleanup();
        return 1;
    }
    printf("Connected to server\n");
    while(1) {
        char buffer[1024];
        fgets(buffer, sizeof(buffer), stdin);
        send(sock, buffer, sizeof(buffer), 0);
    }
    return 0;
}
