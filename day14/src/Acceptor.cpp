#include "../include/Acceptor.h"
#include <assert.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

Acceptor::Acceptor(EventLoop* loop, const char* ip, const int port):loop_(loop), listenfd_(-1){

}

Acceptor::~Acceptor(){
}

void Acceptor::Create(){
    assert(listenfd_ == -1); //避免重复创建
    listenfd_ = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC 0);
    if(listenfd_ == -1){
        std::cout << "create socket error" << std::endl;
    }
}

void Acceptor::Bind(const char* ip, const int port){
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(port);
    if(::bind(listenfd_, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        std::cout << "Failed to Bind: " << ip << ":" << port <<std::endl;
    }
}

void Acceptor::Listen(){
    assert(listenfd_ != -1);
    if(::listen(listenfd_, SOMAXCONN) == -1){
        std::cout << "Failed to Listen" << std::endl;
    }
}

void Acceptor::AcceptConnection(){
    struct sockaddr_int client;
    socklen_t client_addrlength = sizeof(client);
    assert(listenfd_ != -1);

    int clnt_fd = ::accept(listenfd_, (struct sockaddr*)&client, &client_addrlength, SOCK_NONBLOCK | SOCK_CLOEXEC);
    if(clnt_fd == -1){
        std::cout << "Failed to Accept" << std::endl;
    }

    if(new_connection_callback_){
        new_connection_callback_(clnt_fd);
    }
}

void Acceptor::new_connection_callback(std::function<void(int) const &callback){
    new_connection_callback_ = callback;
}