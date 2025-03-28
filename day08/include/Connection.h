#pragma once

#include <functional>

class EventLoop;
class Socket;
class Channel;

class Connection{
private:
    EventLoop* loop;
    Socket* socket;
    Channel* channel;
    std::function<void()> deleteConnectionCallback;
public:
    Connection(EventLoop* _loop, Socket* _socket);
    ~Connection();

    void echo(int sockfd);
    void setDeleteConnectionCallback(std::function<void(Socket*)>);
};