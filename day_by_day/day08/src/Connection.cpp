#include "../include/Connection.h"
#include "../include/Socket.h"
#include "../include/Channel.h"
#include <functional>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop* _loop, Socket* _sock):loop(_loop), sock(_sock), channel(nullptr){
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb); //调用handleRead函数
    channel->enableReading();
}

Connection::~Connection(){
    delete channel;
    delete sock;
}

void Connection::echo(int sockfd){
    char buffer[READ_BUFFER];
    while(true){
        ssize_t bytes_read = read(sockfd, buffer, sizeof(buffer));
        if(bytes_read > 0){
            printf("message from client fd %d: %s \n", sockfd, buffer);
            write(sockfd, buffer, bytes_read);
        }
        else if(bytes_read == -1 && errno == EINTR){
            printf("continue reading\n");
            continue;
        }
        else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){
            printf("EOF, client fd %d disconnected\n", sockfd);
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb){
    deleteConnectionCallback = _cb;
} 
