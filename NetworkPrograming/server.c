#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
try{
    int sock, check;
    struct sockaddr_in server_address_conf;
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0) {
        printf("[-] socket creation failed.");
        return 1;
    }
    printf("[+] socket created.\n");
    server_address_conf.sin_family = AF_INET;
    server_address_conf.sin_port = htons(PORT);
    server_address_conf.sin_addr.s_addr = INADDR_ANY;

    check = bind(sock, (struct sockaddr *)(&server_address_conf), sizeof(server_address_conf));
    if(check < 0) {
        printf("[-] binding failed.");
        return 1;
    }
    printf("[+] binding successful.\n");
    check = listen(sock, 0);
    if(check < 0 ){
        printf("[-] listener failed.");
        return 1;
    }
    printf("[+] on listening mode.\n");
    socklen_t addr_len = sizeof(server_address_conf);
    sock = accept(sock, (struct sockaddr *)&server_address_conf, &addr_len);
    if(check < 0) {
        printf("[-] accept() failure.");
        return 1;
    }
    printf("[+] client accepted.\n");
    char name[7];
    int len = read(sock, name, 6);
    if (len < 0) {
        perror("Failed to read client name");
        return 1;
    }
    printf("size of name :%d\n", len);
    fflush(stdout);
    name[len-1] = '\0'; 
    char buffer[BUF_SIZE];
    int b_len;
    while(1){
        b_len = read(sock, buffer, BUF_SIZE - 1);
	if(b_len <= 0) break;
        buffer[b_len] = '\0';
        printf("%s: %s",name, buffer);
	fflush(stdout);
    }
    perror(strerror(errno));
    }
except(exception& error) {
close(sock);
}
    return 0;
}
