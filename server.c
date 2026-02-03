#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);

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
    if((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Client Connected\n");
    char message[1024];
    while(1){
        if(recv(new_socket, message, sizeof(message), 0) <= 0){
            perror("recv failed");
            close(new_socket);
            break;
        }
        else{
            send(new_socket, message, strlen(message), 0);
        }
    }
    close(new_socket);
}

return 0;
}