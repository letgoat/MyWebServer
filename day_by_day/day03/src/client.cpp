#include "util.h"
#include <sys/socket.h>
#include <sys/epoll.h>
#include <iostream>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main()
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "create socket failed");

    struct sockaddr_in serv_addr;//创建服务器地址，用于连接服务器  
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    errif(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "socket connect failed"); //连接服务器

    while(true){//死循环，一直等待用户输入消息并发送给服务器
        char buffer[BUFFER_SIZE];
        bzero(&buffer, sizeof(buffer));
        scanf("%s", buffer);
        ssize_t write_bytes = write(sockfd, buffer, sizeof(buffer)); //通过sockfd将数据传给服务器
        errif(write_bytes == -1, "write failed");

        bzero(&buffer, sizeof(buffer)); //写完后就清除buffer，避免影响从fd读数据
        ssize_t read_bytes = read(sockfd, buffer, sizeof(buffer)); //从sockfd读取服务器返回的消息（回显）
        if(read_bytes > 0){
            printf("message from server: %s\n", buffer);
        }
        else if(read_bytes == 0){
            printf("server socket disconnected\n");
            break;
        }
        else if(read_bytes == -1){
            close(sockfd);
            errif(true, "socket read error");
        }
    }
    close(sockfd);
    return 0;
}
