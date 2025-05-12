//该类是对事件的轮询和处理，由于每个EventLoop主要不断调用epoll_wait()获取激活的事件，并处理。
//这也就意味着Epoll是独属于EventLoop的成员变量，因此随EventLoop的析构而析构，所以可以采用智能指针。
#pragma once

#include "Epoller.h"
#include "common.h"

class Epoller;
class EventLoop{
public:
    DISALLOW_COPY_AND_MOVE(EventLoop);
    EventLoop();
    ~EventLoop();

    void loop() const;
    void UpdateChannel(Channel* ch) const;
    void RemoveChannel(Channel* ch) const;
    
private:
    std::unique_ptr<Epoller> poller_;
}