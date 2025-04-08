#pragma once

#include <functional>
#include <string>

class EventLoop;
class Socket;
class Channel;
class Buffer;
class Connection{
public:
    enum State{
        Invalid = 1,
        Handshaking,
        Connected,
        Closed,
        Failed,
    };

    Connection(EventLoop* _loop, Socket* _socket);
    ~Connection();

    void Read();
    void Write();

    State GetState();
    
    void setDeleteConnectionCallback(std::function<void(Socket*)> const &callback);
    void setOnConnectionCallback(std::function<void(Connection*)> const &callback);

    void SetSendBuffer(const char* str);
    Buffer* GetReadBuffer();
    const char* ReadBuffer();
    Buffer* GetSendBuffer();
    const char* SendBuffer();
    void GetlineSendBuffer();
    Socket* GetSocket();
    
    Socket* GetSocket() const;
    void Close();

    void OnConnect(std::function<void()> fn);

private:
    EventLoop* loop_; //事件循环
    Socket* sock_; 
    Channel* channel_{nullptr};
    Buffer* read_buffer_{nullptr};
    Buffer* send_buffer_{nullptr};
    State state_{State::Invalid}; //表示连接状态

    std::function<void(Socket*)> delete_connection_callback_;
    std::function<void(Connection*)> on_connection_callback_;
    
    void ReadNonBlocking();
    void WriteNonBlocking();
    void ReadBlocking();
    void WriteBlocking();
};