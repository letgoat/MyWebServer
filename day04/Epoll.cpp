#include "Epoll.h"
#include <sys/epoll.h>
#include <vector>
#include "util.h"
#include <unistd.h>
#include <string.h>

#define MAX_EVENTS 1024

Epoll::Epoll(): epoll_fd(-1),events(nullptr){
    epoll_fd = epoll_create1(0);
    errIf(epoll_fd == -1, "epoll_create1 error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(*events)* MAX_EVENTS); 
}

Epoll::~Epoll(){
    if(epoll_fd != -1){
        close(epoll_fd);
        epoll_fd = -1;
    }
    delete []events;
}

void Epoll::addFd(int fd, uint32_t op){
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    effIf(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll_ctl error");
}

 std::vector<epoll_event> Epoll::poll(int timeout = -1){
    std::vector<epoll_event> activeEvents; //存储活跃事件的数组
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, timeout); //等待epoll事件的发生，并将发生的事件存储在events中，并返回个数epfd，设置超时时间为timeout
    errIf(nfds == -1, "epoll_wait error");
    for(int i = 0; i < nfds; i++){
        activeEvents.push_back(events[i]);
    }
    return activeEvents;
 }
