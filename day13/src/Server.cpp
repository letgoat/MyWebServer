#include "../include/Server.h"
#include "../include/Socket.h"
#include "../include/InetAddress.h"
#include "../include/Channel.h"
#include "../include/Acceptor.h"
#include "../include/Connection.h"
#include "../include/ThreadPool.h"
#include "../include/EventLoop.h"
#include "../include/Connection.h"
#include <functional>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <thread>
#include <future>
#include <assert.h>



#define READ_BUFFER 1024

class Socket;
class InetAddress;
class ThreadPool;

Server::Server(EventLoop* _loop): mainReactor(_loop), acceptor(nullptr){
    acceptor = new Acceptor(mainReactor); //Acceptor只由mainReactor负责
    std::function<void(Socket*)> cb = std::bind(&Server::newConnection, this, std::placeholders::_1);
    //&Server::newConnection是成员函数的指针
    //this作为第一个参数传递给成员函数（即隐式的this参数）
    //std::placeholers::_1是占位符，表示回调函数被调用时的第一个参数
    acceptor->setNewConnectionCallback(cb);

    //先确定线程的数量（硬件并发线程数）
    //然后创建线程池，每一个线程都是一个EventLoop
    //对每一个线程（subReactor[i）都进行事件循环(EventLoop::loop)
    int size = std::thread::hardware_concurrency(); //线程数量，也就是subReactor的数量
    thpool = new ThreadPool(size); //新建线程池
    for(int i = 0; i < size; i++){
        subReactors.push_back(new EventLoop());  //每一个线程都是一个EventLoop
    }

    for(int i = 0; i < size; i++){
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, subReactors[i]);
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
    assert(sock->getFd() != -1);
    int random = sock->getFd() % subReactors.size();
    Connection* conn = new Connection(subReactors[random], sock);
    std::function<void(Socket *)> cb = std::bind(&Server::deleteConnection, this, std::placeholders::_1);
    conn->setDeleteConnectionCallback(cb);
    conn->setOnConnectionCallback(on_connect_callback_);
    connections[sock->getFd()] = conn;
}

void Server::deleteConnection(Socket* sock){
    int sockfd = sock->getFd();
    auto it = connections.find(sockfd);
    if(it != connections.end()){
        Connection* conn = connections[sockfd];
        connections.erase(sockfd);
        delete conn;
        conn = nullptr;
    }
}

void Server::OnConnection(std::function<void(Connection*)> fn){
    on_connect_callback_ = std::move(fn);
}



