#include "../include/EventLoop.h"
#include "../include/Epoll.h"
#include "../include/Channel.h"
#include <vector>

EventLoop::EventLoop():ep(nullptr), quit(false){
    ep = new Epoll();
}

EventLoop::~EventLoop(){
    delete ep;
}

void EventLoop::loop(){
    while(!quit){
        std::vector<Channel*> chs;
        chs = ep->poll();
        for(auto it = chs.begin(); it != chs.end(); it++){ //对每个活跃的channel处理事件
            (*it)->handleEvent();
        }
    }
}