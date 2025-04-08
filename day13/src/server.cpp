#include "../include/Server.h"
#include "../include/EventLoop.h"
#include "../include/Connection.h"
#include "../include/Socket.h"
#include <iostream>

int main(){
    EventLoop *loop = new EventLoop();
    Server *server = new Server(loop);
    server->OnConnection([](Connection* conn){
        conn->Read(); //从客户端读数据
        if(conn->GetState() == Connection::State::Closed){ //如果客户端关闭连接
            conn->Close();
            return;
        }
        std::cout << "Message from client" << conn->GetSocket()->getFd() << ":" << conn->ReadBuffer() << std::endl;
        conn->SetSendBuffer(conn->ReadBuffer()); //将读到的数据写到SendBuffer中
        conn->Write(); //将sendBuffer中的数据写到客户端
    });

    loop->loop();
    delete server;
    delete loop;
    return 0;
}