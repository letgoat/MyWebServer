#include "../include/TcpServer.h"
#include "../include/EventLoop.h"
#include "../include/common.h"

#include <thread>
#include <functional>
#include <assert.h>
#include <iostream>
#include <unistd.h>



TcpServer::TcpServer(const char* ip, const int port): next_conn_id_(1){
    main_reactor_ = std::make_unique<EventLoop>();
    acceptor_ = std::make_unique<Acceptor>(main_reactor_.get(), ip, port);
    std::function<void(int)> cb = std::bind(&TcpServer::HandleNewConnection, this, std::placeholders::_1);
    acceptor_->set_new_connection_callback(cb);

    unsigned int size = std::thread::hardware_concurrency();
    thread_pool_ = std::make_unique<ThreadPool>(size);

    for(size_t i = 0; i < size; i++){
        std::unique_ptr<EventLoop> sub_reactor = std::make_unique<EventLoop>();
        sub_reactors_.push_back(std::move(sub_reactor));
    }
}

TcpServer::~TcpServer(){
}

//将每一个sub_reactor添加到线程池中，并且启动main_reactor
void TcpServer::start(){
    for(int i = 0; i < sub_reactors_.size(); i++){
        std::function<void()> sub_loop = std::bind(&EventLoop::loop, sub_reactors_[i].get());
        thread_pool_->add(std::move(sub_loop));
    }
    main_reactor_->loop();
}

//处理新连接的函数
void TcpServer::HandleNewConnection(int fd){
    assert(fd != -1); //如果fd == -1报错
    std::cout << "New Connection fd: " << fd << std::endl;
    uint64_t random = fd % sub_reactors_.size(); //随意分配一个sub_reactor

    TcpConnection* conn = new TcpConnection(sub_reactors_[random].get(), fd, next_conn_id_++); //分配一个TcpConnection对象

    std::function<void(int)> cb = std::bind(&TcpServer::HandleClose, this, std::placeholders::_1); //设置关闭回调函数cb

    conn->set_close_callback(cb);
    conn->set_message_callback(on_message_);

    connectionsMap_[fd] = conn; //将TcpConnection对象添加到connectionsMap_中
    //分配id
    ++next_conn_id_; 
    if(next_conn_id_ == 1000){ //如果超过1000个连接，则重新分配
        next_conn_id_ = 1;
    }  
}

void TcpServer::HandleClose(int fd){
    //先判断fd是否存在connectionsMap_中
    auto it = connectionsMap_.find(fd);
    assert(it != connectionsMap_.end()); //如果fd不存在报错
    TcpConnection* conn = connectionsMap_[fd];
    connectionsMap_.erase(fd); //从connectionsMap_中删除fd
    ::close(fd); //关闭fd
    conn = nullptr; //释放TcpConnection对象
    std::cout << "Connection fd: " << fd << "closed" << std::endl;
}

void TcpServer::set_connection_callback(std::function<void(TcpConnection *)> const &fn){
    on_connect_ = std::move(fn);
}

void TcpServer::set_message_callback(std::function<void(TcpConnection *)> const &fn){
    on_message_ = std::move(fn);
}

