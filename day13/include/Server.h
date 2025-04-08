#pragma once 
#include <map>
#include <vector>
class EventLoop;
class Socket;
class Acceptor;
class Connection;
class ThreadPool;
class Server{
private:
    EventLoop* mainReactor; //只负责接受连接，然后分发给一个subReactor
    Acceptor* acceptor;  //连接接收器
    std::map<int, Connection*> connections; //TCP连接
    std::vector<EventLoop*> subReactors; //负责处理事件循环
    ThreadPool* thpool; //线程池
    std::function<void(Connection*)> on_connect_callback_; //连接回连函数
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket* sock);
    void deleteConnection(Socket* sock);

    void OnConnection(std::function<void(Connection*)> fn);
};