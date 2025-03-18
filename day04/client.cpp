#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "util.h"
#include "Socket.h"
#include "InetAddress.h"

#define BUFFER_SIZE 1024

int main(){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->connect(*serv_addr);

    while(true){
        char buffer[BUFFER_SIZE];
        bzero(&buffer, BUFFER_SIZE);
        scanf("%s", buffer);
        ssize_t write_bytes = write(serv_sock->fd, buffer, sizeof(buffer));
        if(write_bytes == -1){
            printf("socket already disconnected\n");
            break;
        }
        bzero(&buffer, BUFFER_SIZE);
        ssize_t read_bytes = read(serv_sock->fd, buffer, sizeof(buffer));
        if(read_bytes > 0){
            printf("message from server: %s\n", buffer);
        }
        else if(read_bytes == 0){
            printf("server socket disconnected \n");
            break;
        }
        else if(read_bytes == -1){
            close(serv_sock->fd);
            errIf(true, "socket read error");
        }
    }
    close(serv_sock->fd);
    return 0;
}