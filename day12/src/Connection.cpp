#include "../include/Connection.h"
#include "../include/Socket.h"
#include "../include/Channel.h"
#include "../include/Buffer.h"
#include "../include/util.h"

#include <functional>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define READ_BUFFER 1024

Connection::Connection(EventLoop* _loop, Socket* _sock):loop(_loop), sock(_sock), channel(nullptr), inBuffer(new std::string()), readBuffer(nullptr){
    channel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Connection::echo, this, sock->getFd());
    channel->setCallback(cb); //调用handleRead函数
    channel->enableReading();
    readBuffer = new Buffer();
}

Connection::~Connection(){
    delete channel;
    delete sock;
}

void Connection::echo(int sockfd){
    char buffer[READ_BUFFER]; //这个buffer大小无所谓
    while(true){
        ssize_t bytes_read = read(sockfd, buffer, sizeof(buffer));
        if(bytes_read > 0){
            readBuffer->append(buffer, bytes_read);
        }
        else if(bytes_read == -1 && errno == EINTR){
            printf("continue reading\n");
            continue;
        }
        else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){ //这个条件表示数据全部读取完毕
            printf("finish reading once");
            printf("message from client fd %d: %s\n", sockfd, readBuffer->c_str());
            errif(write(sockfd, readBuffer->c_str(), readBuffer->size()) == -1, "write error");
            break;
        }
        else if(bytes_read == 0){
            printf("EOF, client fd %d disconnected\n", sockfd);
            deleteConnectionCallback(sock);
            break;
        }
    }
}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> _cb){
    deleteConnectionCallback = _cb;
} 
