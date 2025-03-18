#include <stdio.h>
#include <errno.h>
#include <vector>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "Epoll.h"
#include "util.h"
#include "Socket.h"
#include "InetAddress.h"

#define MAX_EVENTS 1024
#define READ_BUFFER 1024

void handleReadEvent(int);
int main(){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->connect(*serv_addr);
    serv_sock->listen();
    Epoll *ep = new Epoll();
    serv_sock->setnonblocking();
    
    return 0;
}

void handleReadEvent(int sockfd){
    char buf[READ_BUFFER];
    while(true){ //由于使用非阻塞IO，读取客户端buffer,一次读取buf大小数据，直到全部读取完毕
        bzero(buf, sizeof);
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            printf("message from client fd %d: %s\n", sockfd, buf);
            write(sockfd, buf, sizeof(buf));//将收到的数据写回客户端
        }
        else if(bytes_read == -1 && errno == EINTR){ //如果errno为EINTR，说明是因为信号导致的，可以继续读取
            printf("continue reading");
            continue;
        }
        else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLICK))){ //如果errno为EAGAIN或EWOULDBLOCK，说明数据已经全部读取完毕
            printf("finish reading once, errno:%d\n", errno);
            break;
        }
        else if(bytes_read == 0){
            printf("EOF, client fd %d closed\n", sockfd);
            close(sockfd);
            break;
        }
    }
}