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
#include <assert.h>
#include <string.h>
Connection::Connection(EventLoop* loop, Socket* sock): loop_(loop), sock_(sock){
    if(loop_ != nullptr){
        channel_ = new Channel(loop_, sock_->getFd());
        channel_->enableRead();
        channel_->useET();
    }
    read_buffer_ = new Buffer();
    send_buffer_ = new Buffer();
    state_ = State::Connected;
}

Connection::~Connection(){
    if(loop_ != nullptr){
        delete channel_;    
    }
    delete sock_;
    delete read_buffer_;
    delete send_buffer_;
}

void Connection::Read(){
    assert(state_ == State::Connected); //确保调用Read()之前，Connected对象是Connected的
    read_buffer_->clear();
    ReadNonBlocking();
}

void Connection::Write(){
    assert(state_ == State::Connected);
    WriteNonBlocking();
    send_buffer_->clear();
}

void Connection::ReadNonBlocking(){
    int sockfd = sock_->getFd();
    char buf[1024];
    while(true){
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(sockfd, buf, sizeof(buf));
        if(bytes_read > 0){
            read_buffer_ -> append(buf, bytes_read);
        }
        else if(bytes_read == -1 && errno == EINTR){ //程序正常中断，继续读取
            printf("continue reading\n");
            continue;
        }
        else if(bytes_read == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK))){ //非阻塞IO，这个条件表示数据全部读取完毕
            break;
        }
        else if(bytes_read == 0){
            printf("read EOF, client fd %d disconnected\n", sockfd);
            state_ = State::Closed;
            break;
        }
        else{
            printf("other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            break;
        }
    }
}

void Connection::WriteNonBlocking(){
    int sockfd = sock_->getFd();
    char buf[send_buffer_->size()];
    memcpy(buf, send_buffer_->c_str(), send_buffer_->size()); //将send_buffer_中的数据复制到buffer中
    int data_size = send_buffer_->size();
    int data_left = data_size;
    while(data_left > 0){
        
    }
}