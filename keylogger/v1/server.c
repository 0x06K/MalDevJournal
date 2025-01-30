#include <sys/socket.h>    // Core socket functions
#include <netinet/in.h>    // Structures for internet addresses
#include <arpa/inet.h>     // Functions for IP address manipulation
#include <unistd.h>        // close() function
#include <string.h>        // memset(), memcpy(), string functions
#include <stdio.h>         // printf(), perror() for debugging
#include <errno.h>         // errno and error-handling functions
#include <cstdlib>
#include <pthread.h>

#define PORT 2005
#define BUF_SIZE 3050

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Initialize mutex

void* listen(void* socket) {
   pthread_mutex_lock(&mutex);
   char buffer[BUF_SIZE] = {0};
   int sock = *(int*)socket;
   int len = 0;
   while((len = read(sock, buffer, sizeof(buffer))) > 0){
	FILE *file;
        const char *filename = "logs.txt";

    // Open the file in write mode ("w")
        file = fopen(filename, "w");
        if (file == NULL) {
           perror("Error opening file");
           return 0;
         }
	buffer[len-1] = '\0';
    // Write data to the file
        fprintf(file, "%s", buffer);  // You can use fprintf, fputs, or fwrite
        printf("\n[+] Data written to file successfully.\n");

    // Close the file
        fclose(file);
   }
   pthread_mutex_unlock(&mutex);
   return 0;
}

int main()
{
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
loop_back:
    // Step 1: Create the socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("[-] Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("[+] Socket created successfully.\n");

    // Step 2: Set socket options (optional, to reuse the port/address)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("[-] setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Step 3: Bind the socket to an IP and port
    address.sin_family = AF_INET;              // IPv4
    address.sin_addr.s_addr = INADDR_ANY;      // Bind to all interfaces
    address.sin_port = htons(PORT);           // Convert port to network byte order

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("[-] Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("[+] Bind successful on port %d.\n", PORT);

    // Step 4: Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("[-] Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("[+] Listening for incoming connections...\n");
    // Step 5: Accept an incoming connection
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (new_socket < 0) {
        perror("[-] Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("[+] Connection accepted from client.\n");

    // Step 6: Send/Receive data (example)
    pthread_t thread;  // Array of threads
    pthread_create(&thread, NULL, listen, &new_socket);
    fflush(stdout);
    while(1){
	char code_word[4096] = {0};
	printf("$:~> ");
	scanf("%s", code_word);
	int check = send(new_socket, code_word, strlen(code_word), 0);
	if((check <= 0)) {
	    close(new_socket);
	    printf("\n[+] connection aborted\n");
	    pthread_cancel(thread);
	    goto loop_back;
	}
	printf("%d", &check);
    }
    // Step 7: Close sockets
    close(new_socket);
    close(server_fd);

    return 0;



}
