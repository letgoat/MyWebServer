#include "../include/Server.h"
#include "../include/Socket.h"
#include "../include/InetAddress.h"
#include "../include/Channel.h"
#include <functional>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#define READ_BUFFER 1024

class Socket;
class InetAddress;

Server::Server(EventLoop* _loop): loop(_loop){
    Socket *serv_sock = new Socket();
    InetAddress *serv_addr = new InetAddress("127.0.0.1", 8888);
    serv_sock->bind(serv_addr);
    serv_sock->listen();
    serv_sock->setnonblocking();

    Channel *servChannel = new Channel(loop, serv_sock->getFd());
    std::function<void()> cb = std::bind(&Server::newConnection, this, serv_sock);
    //此时调用cb(); 相当于--> Server::newConnetion(serv_sock);
    servChannel->setCallback(cb);
    servChannel->enableReading();
}

Server::~Server(){

}

void Server::newConnection(Socket* serv_sock){
    InetAddress *client_addr = new InetAddress(); //会出现内存泄漏，没有delete
    Socket *client_sock = new Socket(serv_sock->accept(client_addr)); //会出现内存泄漏，没有delete
    printf("new client connected IP: %s port: %d\n", client_addr->addr.sin_addr, client_addr->addr.sin_port);
    client_sock->setnonblocking();
    Channel *client_Channel = new Channel(loop, client_sock->getFd());
    std::function<void()> cb = std::bind(Server::handleReadEvent, this, client_sock->getFd());
    client_Channel->setCallback(cb);
    client_Channel->enableReading();
}

void Server::handleReadEvent(int sockfd){
    char buffer[READ_BUFFER];
    while(true){ //由于使用非阻塞IO,读取客户端buffer,一次读取buffer大小数据，知道全部读取完毕
        bzero(&buffer, sizeof(buffer));
        ssize_t read_bytes = ::read(sockfd, buffer, sizeof(buffer));
        if(read_bytes > 0){
            printf("message from client fd %d: %s\n", sockfd, buffer);
            write(sockfd, buffer, read_bytes);
        }
        else if(read_bytes == -1 && errno == EINTR){ //客户端中断，重新读取
            printf("continue reading\n");
            continue;
        }
        else if(read_bytes == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)){ //数据全部读取完毕
            printf("client fd %d read finish\n", sockfd);
            break;
        }
        else if(read_bytes == 0){ //EOF, 客户端断开连接
            printf("client fd %d disconnected\n", sockfd);
            close(sockfd); //关闭socket，会自动将文件描述符从epoll中移除
            break;
        }
    }
}



