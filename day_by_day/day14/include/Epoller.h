//主要是进行多路复用，保证高并发
//在Epoller类主要是对epoll中channel的监听和处理

#pragma once
#include "common.h"
#include <vector>

class Channel;
//私有成员：fd_, struct epoll_event *events_(从epoll_wait中获取事件)
//成员函数：更新监听的channel, 删除监听的channel, 返回调用完epoll_wait的通道事件
class Epoller{
public:
    DISALLOW_COPY_AND_MOVE(Epoller);

    Epoller();
    ~Epoller();

    void updateChannel(Channel *ch) const;
    void removeChannel(Channel *ch) const;

    std::vector<Channel*> Poll(long timeout = -1) const;
private:
    int fd_;
    struct epoll_event *events_;
};