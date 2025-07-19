#include "EventLoopThreadPool.h"
#include "EventLoop.h"
#include "EventLoopThread.h"
#include <memory>
#include <thread>

EventLoopThreadPool::EventLoopThreadPool(EventLoop *loop)
    : main_reactor_(loop),
      thread_nums_(0),
      next_(0){};

EventLoopThreadPool::~EventLoopThreadPool(){}


//启动线程池
void EventLoopThreadPool::start(){
    for (int i = 0; i < thread_nums_; ++i){ 
        std::unique_ptr<EventLoopThread> ptr = std::make_unique<EventLoopThread>();
        threads_.push_back(std::move(ptr)); //将事件线程(EventLoopThread) 的 指针放入容器threads_中
        loops_.push_back(threads_.back()->StartLoop()); //启动事件线程，并获取事件，放入loops_容器中
    }
}

EventLoop *EventLoopThreadPool::nextloop(){
    EventLoop *ret = main_reactor_;
    if (!loops_.empty()){
        ret = loops_[next_++];
        if (next_ == static_cast<int>(loops_.size())){
            next_ = 0;
        }
    }
    return ret;
}

void EventLoopThreadPool::SetThreadNums(int thread_nums){
    thread_nums_ = thread_nums;
}