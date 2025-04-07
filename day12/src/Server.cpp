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

Server::Server(EventLoop* _loop): mainReactor(_loop), acceptor(nullptr){
    acceptor = new Acceptor(mainReactor); //Acceptor只由mainReactor负责
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    acceptor->setNewConnectionCallback(cb);

    int size = std::thread::hardware_concurrency(); //线程数量，也就是subReactor的数量
    thpool = new ThreadPool(size); //新建线程池
    for(int i = 0; i < size; i++){
        subRecactors.push_back(new EventLoop());  //每一个线程池都是一个EventLoop
    }

    for(int i = 0; i < size; i++){
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactor[i]);
        thpool->add(sub_loop); //开启所有线程事件的循环
    }   
}

Server::~Server(){
    delete acceptor;
    delete thpool;
}

void Server::handleReadEvent(int ){

}

void Server::newConnection(Socket* sock){
    if(sock->getFd() != -1){
        int random = sock->getFd() % subReactors.size();
        Connection* conn = new Connection(subReactors[random], sock);
        std::function<void(int)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
        conn->setDeleteConnectionCallback(cb);
        connection[sock->getFd()] = conn;
    }
}

void Server::delteConnection(int sockfd){
    if(sockfd != -1){
        auto it = connections.find(sockfd);
        if(it != connections.end()){
            Connection* conn = connections[sockfd];
            connections.erase(sockfd);
            delete conn;
        }
    }
}




