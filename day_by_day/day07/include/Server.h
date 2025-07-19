#pragma once 

class EventLoop;
class Socket;
class Channel;
class Server{
private:
    EventLoop* loop;
public:
    Server(EventLoop*);
    ~Server();

    void handleReadEvent(int);
    void newConnection(Socket* serv_sock);

};