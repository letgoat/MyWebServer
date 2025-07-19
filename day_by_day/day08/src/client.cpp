#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "../include/util.h"

#define BUFFER_SIZE 1024

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "create socket failed");

    struct sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8888);

    errif(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect failed");

    while(true){
        char buffer[BUFFER_SIZE];
        bzero(buffer, sizeof(buffer));
        scanf("%s", buffer);
        ssize_t write_bytes = write(sockfd, buffer, sizeof(buffer));
        if(write_bytes == -1){
            errif(true, "write error, can't write any more");
            break;
        }
        bzero(buffer, sizeof(buffer));
        ssize_t read_bytes = read(sockfd, buffer, sizeof(buffer));
        if(read_bytes == -1){
            close(sockfd);
            printf("socket read error");
        }
        else if(read_bytes == 0){
            printf("server disconnected");
            break;
        }
        else if(read_bytes > 0){
            printf("message from server: %s", buffer);
        }

    }
    close(sockfd);
    return 0;
}