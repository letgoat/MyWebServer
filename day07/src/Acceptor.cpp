#include "../include/Acceptor.h"
#include "../include/InetAddress.h"
#include "../include/Socket.h"
#include "../include/Channel.h"
#include "../include/EventLoop.h"

Acceptor::Acceptor(Eventloop *_loop): loop(_loop){
    sock = new Socket();
    addr = new InetAddress("127.0.0.1", 8888);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    acceptChannel = new Channel(loop, sock->getFd());
    std::function<void()> cb = std::bind(&Acceptor::acceptConnection, this);
    acceptChannel->setCallback(cb);
    acceptChannel->enableReading();

}

