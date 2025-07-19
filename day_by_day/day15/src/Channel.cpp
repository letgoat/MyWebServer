#include "../include/Channel.h"
#include <unistd.h>
#include <sys/epoll.h>
//构造函数
//进行简单列表初始化即可
Channel::Channel(int fd, EventLoop* loop): fd_(fd), loop_(loop), listen_events_(0), ready_events_(0), IsInEpoll(false){
}

//析构函数
//关闭文件，并且将文件描述符置为-1
Channel::~Channel(){
    if(fd_ != -1){
        ::close(fd_);
        fd_ = -1;
    }
}

//事件处理函数
//读取相关的事件触发读回调
//写相关的事件触发写回调
void Channel::HandleEvent() const{
    if(ready_events_ & (EPOLLIN | EPOLLPRI | EPOLLRDHUP))
    {
        if(read_callback_){
            read_callback_();
        }
    }
    if(ready_events_ & EPOLLOUT){
        if(write_callback_){
            write_callback_();
        }
    }
}

void Channel::EnableRead(){
    listen_events_ |= (EPOLLIN | EPOLLPRI);
    loop_->update_channel(this);
}

void Channel::EnableWrite(){
    listen_events_ |= EPOLLOUT;
    loop_->update_channel(this);
}

void Channel::EnableET(){
    listen_events_ |= EPOLLET;
    loop_->update_channel(this);
}

void Channel::DisableWrite(){
    listen_events_ &= ~EPOLLOUT;
    loop_->update_channel(this);
}

int Channel::fd() const{
    return fd_;
}

short Channel::listen_events() const{
    return listen_events_;
}

short Channel::ready_events() const{
    return ready_events_;
}

bool Channel::IsInEpoll() const{
    return in_epoll_;
}

void Channel::SetInEpoll(bool in){
    in_epoll_ = true;
}

void Channel::SetReadyEvents(int ev){
    ready_events_ = ev;
}

void Channel::set_read_callback(const std::function<void()> &callback){
    read_callback_ = std::move(callback);
}

void Channel::set_write_callback(const std::function<void()> &writeback){
    write_callback_ = std::move(writeback);
}