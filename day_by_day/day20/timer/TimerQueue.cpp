#include "TimerQueue.h"
#include "Timer.h"
#include <unistd.h>
#include <sys/timerfd.h>
#include <iostream>

TimerQueue::TimerQueue(EventLoop *loop):loop_(loop){
    CreateTimerfd();
    channel_ = std::make_unique<Channel>(timerfd_, loop_);
    channel_->setReadCallback(std::bind(&TimerQueue::HandleRead, this));
    channel_->enableReading();
}

TimerQueue::~TimerQueue(){
    loop_->removeChannel(channel_.get());
    close(timerfd_);
    for(const auto & entry: timers_){
        delete entry.second;
    }
}

void TimerQueue::CreateTimerfd(){
    timerfd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    if(timerfd_ < 0){
        std::cout << "Failed in timerfd_create" << std::endl;
    }
}

void TimerQueue::ReadTimerfd(){
    uint64_t read_byte;
    ssize_t readn = ::read(timerfd_, &read_byte, sizeof(read_byte));
    if(readn != sizeof(read_byte)){ //检查读取的数据是否是八字节
        std::cout << "TimeQueue::ReadTimerfd read error" << std::endl;
    }
}

void TimerQueue::HandleRead(){
    ReadTimerfd();
    active_timers_.clear();

} 

void ResetTimerFd(Timer *timer){

} 

void ResetTimers(){

}

bool TimerQueue::Insert(Timer *timer){
    bool reset_instantly = false;
    
} 

void TimerQueue::AddTimer(TimeStamp timestamp, std::function<void()> const & cb, double interval){
    Timer *timer = new Timer(timestamp, cb, interval);

}