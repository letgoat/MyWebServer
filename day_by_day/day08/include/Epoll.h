#pragma once
#include <sys/epoll.h>
#include <vector>

class Channel;
class Epoll{
private:
    int epfd;
    struct epoll_event* events;
public:
    Epoll();
    ~Epoll();

    void addFd(int fd, uint32_t op); //将新的fd添加入epoll中
    void updateChannel(Channel*); //更新某个channel的状态

    std::vector<Channel*> poll(int timeout = -1); //等待事件发生，并返回活跃的Channel列表
};