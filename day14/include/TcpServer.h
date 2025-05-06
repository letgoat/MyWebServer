//TcpServer是对整个服务器的管理，他通过创建acceptor来接收连接。
//并管理TcpConnection的添加
//在这个类中，由于TcpConnection的生命周期比较模糊，暂时使用裸指针，后续改为智能指针

#pragma once
#include <common.h>
#include <functional>

class TcpConnection;
class Acceptor;
class EventLoop;
class ThreadPool;

class TcpServer{
public:

private:


};