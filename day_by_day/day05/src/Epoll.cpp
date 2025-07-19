#include "../include/../include/Epoll.h"
#include "../include/../include/util.h"
#include "../include/Channel.h"
#include <unistd.h>
#include <string.h>

#define MAX_EVENTS 1000

Epoll::Epoll():epfd(-1), events(nullptr){
    epfd = epoll_create1(0);
    errIf(epfd == -1, "epoll_create1 error");
    events = new epoll_event[MAX_EVENTS];
    bzero(events, sizeof(epoll_event)*MAX_EVENTS);
}

Epoll::~Epoll(){
    if(epfd != -1){
        close(epfd);
        epfd = -1;
    }
    delete[] events;
}

void Epoll::addFd(int fd, uint32_t op){
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.events = op;
    ev.data.fd = fd;
    errIf(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epoll_ctl error");
}

void Epoll::updateChannel(Channel *channel){
    int fd = channel->getFd();
    struct epoll_event ev;
    bzero(&ev, sizeof(ev));
    ev.data.ptr = channel;
    ev.events = channel->getEvents();
    if(!channel->isInEpoll()){ //如果channel不在epoll中，则添加到epoll中
        errIf(epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1, "epool_ctl error");
        channel->setInEpoll();
    }
    else{
        errIf(epoll_ctl(epfd, EPOLL_CTL_MOD, fd, &ev) == -1, "epoll_ctl error");
    }
}

std::vector<Channel*> Epoll::poll(int timeout){
    std::vector<Channel*> activeChannels;
    int nfds = epoll_wait(epfd, events, MAX_EVENTS, timeout);
    errIf(nfds == -1, "epoll_wait error");
    for(int i = 0; i < nfds; i++){
        Channel *ch = (Channel*)events[i].data.ptr;
        ch->setRevents(events[i].events);
        activeChannels.push_back(ch);
    }
    return activeChannels;
}