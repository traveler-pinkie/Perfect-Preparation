#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define MAX_CLIENTS 100

struct client_info{
    int fd;
    char buffer[1024];
    int buff_len;
    char nickname[64];   
};

struct client_info clients[MAX_CLIENTS];
int client_count = 0;



int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    fd_set readfds;


    for(int i = 0; i < MAX_CLIENTS; i++){
    clients[i].fd = -1;
    }



    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket failed");
    exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if(listen(server_fd, 3) < 0){
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    while(1){

        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        if((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0){
            perror("accept failed");
            exit(EXIT_FAILURE);
        }
        printf("Client Connected\n");
        char message[1024];
        int bytes_received;
        while(1){
            bytes_received = recv(new_socket, message, sizeof(message) - 1, 0);
            if(bytes_received <= 0){
                perror("recv failed");
                close(new_socket);
                break;
            }
            message[bytes_received] = '\0';
            printf("Received: %s\n", message);
            send(new_socket, message, bytes_received, 0);
        }
        close(new_socket);
}

return 0;
}