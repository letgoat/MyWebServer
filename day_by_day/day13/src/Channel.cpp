#include "../include/Channel.h"
#include "../include/EventLoop.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>

class EventLoop;

Channel::Channel(EventLoop* _loop, int _fd): loop(_loop), fd(_fd), events(0), ready(0), inEpoll(false){
}

Channel::~Channel(){
    if(fd != -1){
        close(fd);
        fd = -1;
    }
}

void Channel::handleEvent(){
    if(ready & (EPOLLIN | EPOLLPRI)){ //EPOLLIN表示通道可读，EPOLLPRI表示有紧急数据可读取
        readCallback();
    }
    if(ready & EPOLLOUT){ //表述通道可写，即可以向通道发送数据
        writeCallback();
    }
}

void Channel::enableRead(){
    events |= EPOLLIN | EPOLLPRI; //将EPOLLIN和EPOLLPRI事件加入到events中
    loop->updateChannel(this); //事件循环该Channel关心的事件类型已经更新，需要重新在事件循环中设置或更新
}

void Channel::useET(){
    events |= EPOLLET; //添加EPOLLET事件到events中，边缘触发模式：只有在文件描述符发生改变（不可读变为可读），epoll_wait会返回该文件描述符
    //而在水平触发模式下，只要文件描述符状态满足条件（例如可读），epoll_wait就会一直返回该文件描述符，直到事件被处理。
    loop->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}

uint32_t Channel::getReady(){
    return ready;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(bool _in){
    inEpoll = _in;
}

void Channel::setReady(uint32_t _ev){
    ready = _ev;
}

void Channel::setReadCallback(std::function<void()> _cb){
    readCallback = _cb;
}