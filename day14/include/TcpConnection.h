//对于每个TCP连接，都可以用一个类进行管理
//在这个类中，将注意力转移到对客户端socket的读写上
//还需要绑定几个回调函数，例如接收到信息时，或者需要关闭时进行的操作
//并且新增加一个conn_id成员变量，主要是fd可能被复用，在debug时，可以更清晰的追寻问题

#pragma once
#include "common.h"

class Buffer;
class TcpConnection{
public:
    enum ConnectionState{
        Invalid = 1,
        Connected,
        Disconnected
    };

    DISALLOW_COPY_AND_MOVE(TcpConnection);

    TcpConnection(EventLoop* loop, int connfd, int connid);
    ~TcpConnection();

    //关闭时的回调函数
    void set_close_callback(std::function<void(int)> const &fn);

    //接受信息的回调函数
    void set_message_callback(std::function<void(TcpConnection*)> const &fn);

    //设定send buf
    void set_send_buffer(const char* str);
    Buffer* read_buf();
    Buffer* write_buf();

    void Read();
    void Write();
    void Send(const std::string& msg);
    void Send(const char* msg);

    void HandleMessage(); //当接收到信息时，进行回调

    //当TcpConnection发起关闭请求时，进行回调，释放相应的Socket
    void HandleClose();

    ConnectionState state() const;
    EventLoop* loop() const;
    int fd() const;
    int id() const;

private:
    //该连接绑定的socket
    int connfd_;
    int connid_; //文件描述符connfd_可能被复用，所以增加一个connid_来标识唯一连接

    //连接状态
    ConnectionState state_;

    EventLoop* loop_;

    std::unique_ptr<Channel> channel_;
    std::unique_ptr<Buffer> read_buffer_;
    std::unique_ptr<Buffer> send_buffer_;

    std::function<void(int)> on_close_;
    std::function<void(TcpConnection *)> on_message_;

    void ReadNonBlocking();
    void WriteNonBlocking();
};