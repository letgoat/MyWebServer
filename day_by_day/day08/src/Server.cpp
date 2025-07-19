#include "../include/Server.h"
#include "../include/Socket.h"
#include "../include/InetAddress.h"
#include "../include/Channel.h"
#include "../include/Acceptor.h"
#include "../include/Connection.h"
#include <functional>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>


#define READ_BUFFER 1024

class Socket;
class InetAddress;

Server::Server(EventLoop* _loop): loop(_loop), acceptor(nullptr){
   acceptor = new Acceptor(loop);
   std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
   acceptor->setNewConnectionCallback(cb);
}

Server::~Server(){
    delete acceptor;
}

void Server::handleReadEvent(int ){

}

void Server::newConnection(Socket* sock){
    Connection *conn = new Connection(loop, sock);
    std::function<void(Socket*)> cb = std::bind(&Server::delteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    connections[sock->getFd()] = conn;
}

void Server::delteConnection(Socket* sock){
    Connection* conn = connections[sock->getFd()];
    connections.erase(sock->getFd());
    delete conn;
}




