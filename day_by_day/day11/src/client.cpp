#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "../include/util.h"
#include "../include/Socket.h"
#include "../include/InetAddress.h"
#include "../include/Buffer.h"


#define BUFFER_SIZE 1024

int main(){
    Socket* sock = new Socket();
    InetAddress* addr = new InetAddress("127.0.0.1", 1234);
    sock->connect(addr);

    int sockfd = sock->getFd();

    Buffer* send_Buffer = new Buffer();
    Buffer* read_Buffer = new Buffer();

    while(true){
        send_Buffer->getline();
        ssize_t write_bytes = write(sockfd, send_Buffer->c_str(), send_Buffer->size());
        if(write_bytes == -1){
            errif(true, "write error, can't write any more");
            break;
        }
        int already_read = 0;
        char buffer[1024]; //这个buffer的大小无所谓
        while(true){
            bzero(&buffer, sizeof(buffer));
            ssize_t read_bytes = read(sockfd, buffer, sizeof(buffer));
            if(read_bytes > 0){
                read_Buffer->append(buffer, read_bytes);
                already_read += read_bytes;
            }
            else if(read_bytes == 0){
                printf("server disconnected\n");
                exit(EXIT_SUCCESS);
            }
            if(already_read >= send_Buffer->size()){
                 printf("message from server: %s\n", read_Buffer->c_str());
                break;
            }
        }
        read_Buffer->clear();
    }
    delete sock;
    delete addr;
    return 0;
}