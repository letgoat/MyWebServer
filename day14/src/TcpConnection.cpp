#include "../include/TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* loop, int connfd, int connid):loop_(loop), connfd_(connfd), connid_(connid){
    if(loop != nullptr){
        channel_ = std::make_unique<Channel>(connfd, loop);
        channel_->EnableET();
        channel_->set_read_callback(std::bind(&TcpConnection::HandleMessage, this));
        channel_->EnableRead();
    }
    read_buf_ = std::make_unique<Buffer>();
    send_buf_ = std::make_unique<Buffer>();
}

TcpConnection::~TcpConnection(){
    ::close(connfd_);
}

void TcpConnection::set_close_callback(std::function<void(int)> const &fn){
    on_close_ = std::move(fn);
}

void TcpConnection::set_message_callback(std::function<void(TcpConnection *)> const &fn){
    on_message_ = std::move(fn);
}

void TcpConnection::HandleMessage(){
    Read();
    if(on_message_){
        on_message_(this);
    }
}

void TcpConnection::HandleClose(){
    if(state_ != ConnectionState::Disconnected){
        state_ = ConnectionState::Disconnected;
        if(on_close_){
            on_close_(connfd_);
        }
    }
}

EventLoop *TcpConnection::loop() const {return loop_;}
int fd() const{return connfd_;}
int id() const{return connid_;}
TcpConnection::ConnectionState TcpConnection::state() const{return state_;}
void TcpConnection::set_send_buf(const char* buf){send_buffer_->set_buf(buf);}
Buffer *TcpConnection::read_buffer() {return read_buf_.get();}
Buffer *TcpConnection::send_buffer() {return send_buf_.get();}

void TcpConnection::Send(const std::string& msg){
    set_send_buf(msg.c_str());
    Write();
}

void TcpConnection::Send(const char* msg){
    set_send_buf(msg);
    Write();
}

void TcpConnection::Read(){
    read_buf_->Clear();
    ReadNonblocking();
}

void TcpConnection::Write(){
    WriteNonBlocking();
    send_buf_->Clear();
}

void TcpConnection::ReadNonBlocking(){
    char buf[1024];
    while(true){
        memset(buf, 0, sizeof(buf));
        ssize_t bytes_read = read(connfd_, buf, sizeof(buf));
        if(bytes_read > 0){
            read_buf_->Append(buf, bytes_read);
        }
        else if()
    }
}

void HandleMessage(){

}