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
    int data_left = data_size; //data_left是剩余数据量
    while(data_left > 0){
        ssize_t bytes_write = write(sockfd, buf + data_size - data_left, data_left);
        if(bytes_write == -1 && errno == EINTR){
            printf("continue writing\n");
            continue;
        }
        if(bytes_write == -1 && errno == EAGAIN){
            break;
        }
        if(bytes_write == -1){
            printf("other error on client fd %d\n", sockfd);
            state_ = State::Closed;
            break;
        }
        data_left -= bytes_write;
    }
}

void Connection::Close(){delete_connection_callback_(sock_);}


Connection::State Connection::GetState(){return state_;}
void Connection::SetSendBuffer(const char* str){send_buffer_->setBuf(str);}
Buffer* Connection::GetReadBuffer(){return read_buffer_;}
const char *Connection::ReadBuffer(){return read_buffer_->c_str();}
Buffer* Connection::GetSendBuffer(){return send_buffer_;}
const char* Connection::SendBuffer(){return send_buffer_->c_str();}

void Connection::setDeleteConnectionCallback(std::function<void(Socket*)> const& callback){
    delete_connection_callback_ = callback;
}

void Connection::setOnConnectionCallback(std::function<void(Connection*)> const& callback){
    on_connection_callback_ = callback;
    channel_->setReadCallback([this](){on_connection_callback_(this);});
}

void Connection::GetlineSendBuffer(){send_buffer_->getline();}

Socket* Connection::GetSocket() const {return sock_;}

