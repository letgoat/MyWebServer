#include "../include/TcpServer.h"
#include "../include/EventLoop.h"
#include "../include/common.h"

#include <thread>

TcpServer::TcpServer(const char* ip, const int port): next_conn_id_(1){
    main_reactor_ = std::make_unique<EventLoop>();
    acceptor_ = std::make_unique<Acceptor>(main_reactor_.get(), ip, port);
    std::function<void(int)> cb = std::bind(&TcpServer::HandleNewConnection, this, std::placeholders::_1);
    acceptor_->set_new_connection_callback(cb);

    unsigned int size = std::thread::hardware_concurrency();
    thread_pool_ = ;

    for(size_t i = 0; i < size; i++){
        std::unique_ptr<EventLoop> sub_reactor = std::make_unique<EventLoop>();
        sub_reactors_.push_back(std::move(sub_reactor));
    }
}