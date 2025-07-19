#include "../include/Acceptor.h"
#include "../include/InetAddress.h"
#include "../include/Socket.h"
#include "../include/Channel.h"
#include "../include/EventLoop.h"
#include <stdio.h>

class Channel;

Acceptor::Acceptor(EventLoop *_loop): loop(_loop), sock(nullptr), acceptChannel(nullptr){
    sock = new Socket();
    InetAddress* addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();
    delete addr;
}

Acceptor::~Acceptor(){
    delete sock;
    delete acceptChannel;
}

void Acceptor::acceptConnection(){
    InetAddress* clnt_addr = new InetAddress();
    Socket* clnt_sock = new Socket(sock->accept(clnt_addr));
    printf("new client fd %d! IP: %s Port: %d\n", clnt_sock->getFd(), clnt_addr->addr.sin_addr, clnt_addr->addr.sin_port);
    clnt_sock->setnonblocking();
    newConnectionCallback(clnt_sock);
    delete clnt_addr;
}

void Acceptor::setNewConnectionCallback(std::function<void(Socket*)> _cb){
    newConnectionCallback = _cb;
}
