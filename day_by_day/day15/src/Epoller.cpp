#include "../include/Epoller.h"
#include <sys/epoll.h>
#include <string.h>
#include <iostream>

#define MAXEVENTS 1024

Epoller::Epoller(){
    fd_ = epoll_create(0);
    events_ = new epoll_event[MAXEVENTS];
    memset(events_, 0, sizeof(*events_)*MAXEVENTS);
};

Epoller::~Epoller(){
    //释放events_空间
    delete[] events_;
    //关闭socket
    if(fd_ != -1){
        close(fd_);
        fd_ = -1;
    }
}

//使用channel对其监听的事件进行update(epoll_ctl)
void Epoller::updateChannel(Channel* ch) const{
    int sockfd = ch->fd();
    struct epoll_event ev;
    ev.date.ptr = ch;
    ev.events = ch->listenEvents();
    if(!ch->IsInepoll()){
        if(epoll_ctl(fd_, EPOLL_CTL_ADD, sockfd, &ev) == -1){
            std::cout << "Epoller::UpdateChannel epoll_ctl_add failed" << std::endl;
        }
        else ch->SetInepoll(true);
    }
    else{
        if(epoll_ctl(fd_, EPOLL_CTL_MOD, sockfd, &ev) == -1){
            std::cout << "Epoller::UpdateChannel epoll_ctl_mod failed" << std::endl;
        }
    }
}

//将监听该事件的channel从epoll中移除
void Epoller::removeChannel(Channel* ch) const{
    int sockfd = ch->fd();
    if(epoll_ctl(fd_, EPOLL_CTL_DEL, sockfd, NULL) == -1){
        std::cout << "Epoller::RemoveChannel epoll_ctl_del failed" << std::endl;
    }
    else ch->SetInepoll(false);
}

//首先获得目前有哪些就绪的事件(events_数组)
//遍历将每个事件找一个channel监视起来，并且将channel放入vector中
std::vector<Channel*> Epoller::Poll(long timeout) const{
    vector<Channel*> active_channels;
    int nfds = epoll_wait(fd_, events_, MAXEVENTS, timeout);
    if(nfds == -1) perror("Epoller::Poll epoll_wait error");
    for(int i = 0; i < nfds; i++){
        Channel* ch = (Channel*) events_[i].data.ptr;
        ch->SetReadyEvents(events_[i].events);
        active_channels.emplace_back(ch);
    }
    return active_channels;
}