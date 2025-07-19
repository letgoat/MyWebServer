#include "../include/EventLoop.h"

EventLoop::EventLoop(){
    poller_ = std::make_unique<Epoller>();
}

EventLoop::~EventLoop(){
}

void EventLoop::loop() const{
    while(true){
        for(Channel* active_ch : poller_->Poll()){
            active_ch->HandleEvent();
        }
    }
}

void EventLoop::UpdateChannel(Channel* ch) const{
    poller_->updateChannel(ch);
}

void EventLoop::RemoveChannel(Channel* ch) const{
    poller_->removeChannel(ch);
}
