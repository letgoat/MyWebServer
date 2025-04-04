#include "../include/Channel.h"
#include "../include/EventLoop.h"

class EventLoop;

Channel::Channel(EventLoop* _loop, int _fd): loop(_loop), fd(_fd), events(0), revents(0), inEpoll(false){
}

Channel::~Channel(){
}

void Channel::handleEvent(){
    callback();
}

void Channel::enableReading(){
    events = EPOLLIN | EPOLLET;
    loop->updateChannel(this);
}

int Channel::getFd(){
    return fd;
}

uint32_t Channel::getEvents(){
    return events;
}

uint32_t Channel::getRevents(){
    return revents;
}

bool Channel::getInEpoll(){
    return inEpoll;
}

void Channel::setInEpoll(){
    inEpoll = true;
}

void Channel::setRevents(uint32_t _revents){
    revents = _revents;
}

void Channel::setCallback(std::function<void()> _callback){
    callback = _callback;
}