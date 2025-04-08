#include "../include/Epoll.h"
#include "../include/util.h"
#include "../include/Channel.h"
#include <string.h>
#include <unistd.h>
#include <vector>

#define MAX_EVENTS 1000

class Channel;

Epoll::Epoll(): epfd(-1), events(nullptr){
    epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create error");
    events = new epoll_event[MAX_EVENTS];
    bzero(&events, sizeof(epoll_event)*MAX_EVENTS);
}

Epoll::~Epoll(){
    if(epfd != -1){
        epfd = -1;
        close(epfd);
    }
    delete []events;
}

void Epoll::addFd(int fd, uint32_t op){
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.fd = fd;
    ev.events = op;
    errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
}

std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout); //等待的事件都放到events数组中
    errif(nfds == -1, "epoll wait error");
    for(int i = 0; i < nfds; i++){ //给events数组中的每一个事件都找到对应的channel
        Channel* ch = static_cast<Channel*>(events[i].data.ptr);
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
}

// 核心功能：将channel注册或更新到epoll中，使其能监听文件描述符fd上的事情，具体来说：
// - 若channel未添加到epoll中，则执行EPOLL_CTL_ADD添加
// - 若channel已添加到epoll中，则执行EPOLL_CTL_MOD更新其监听事件
void Epoll::updateChannel(Channel* channel){
    int fd = channel->getFd(); //所有的epoll操作均基于此fd
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents(); //从channel中获取其关注的事件（EPOLLIN, EPOLLOUT)
    if(!channel->getInEpoll()){
        errif(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll add event error");
        channel->setInEpoll();
    }
    else{
        errif(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll modify event error");
    }
}