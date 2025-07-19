#include "EventLoop.h"

#include "Channel.h"
#include "Epoller.h"
#include "CurrentThread.h"
#include "TimerQueue.h"
#include "TimeStamp.h"
#include <memory>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <assert.h>


EventLoop::EventLoop() : tid_(CurrentThread::tid()) { 
    // 将Loop函数分配给了不同的线程，获取执行该函数的线程
    poller_ = std::make_unique<Epoller>();
    timer_queue_ = std::make_unique<TimerQueue>(this);
    wakeup_fd_ = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
    wakeup_channel_ = std::make_unique<Channel>(wakeup_fd_, this);
    calling_functors_ = false;

    wakeup_channel_->set_read_callback(std::bind(&EventLoop::HandleRead, this));
    wakeup_channel_->EnableRead();
}

EventLoop::~EventLoop() {
    DeleteChannel(wakeup_channel_.get());
    ::close(wakeup_fd_);
}


// I/O处理与业务逻辑解耦
void EventLoop::Loop(){
    while (true)
    {   //1.处理I/O事件(立即响应，短时间完成)
        //由epoll等系统调用触发，仅在事件循环线程处理
        for (Channel *active_ch : poller_->Poll()){
            active_ch->HandleEvent();
        }
        //2.处理任务事件(可延迟执行，允许耗时)
        //可由任何线程提交(工作线程，其他事件循环等)
        DoToDoList();
    }
}

void EventLoop::UpdateChannel(Channel *ch) { poller_->UpdateChannel(ch); }
void EventLoop::DeleteChannel(Channel *ch) { poller_->DeleteChannel(ch); }

bool EventLoop::IsInLoopThread(){
    return CurrentThread::tid() == tid_;
}

void EventLoop::RunOneFunc(std::function<void()> cb){
    if(IsInLoopThread()){
        cb();
    }else{
        QueueOneFunc(cb);
    }
}


// EventLoop::QueueOneFunc 函数的主要功能是将一个回调函数添加到事件循环的任务队列中，
// 并在当前线程不是事件循环线程的情况下，通过向 wakeup_fd_ 写入数据来唤醒事件循环线程，
// 以便其处理新的任务。这使得任何线程都可以向事件循环提交任务，而不必担心线程同步问题。
void EventLoop::QueueOneFunc(std::function<void()> cb){
    {
        // 加锁，保证线程同步
        std::unique_lock<std::mutex> lock(mutex_);
        to_do_list_.emplace_back(std::move(cb));
    }

    //main reactor线程和sub reactor线程都可能调用EventLoop

    //条件!IsInLoopThread() --> 比如工作线程调用EventLoop::QueueOneFunc
    //条件calling_functors_ --> 当前正在执行任务队列中的函数
    if (!IsInLoopThread() || calling_functors_) { //如果不是当前事件循环线程或者正在执行其他任务，就需要唤醒事件循环线程来处理新的任务。
        uint64_t write_one_byte = 1; // 向wakeup_fd_写入一个字节，唤醒事件循环线程
        ssize_t write_size = ::write(wakeup_fd_, &write_one_byte, sizeof(write_one_byte));
        (void) write_size;
        assert(write_size == sizeof(write_one_byte));
    }
}


// 该函数的实现理念：交换两个容器，将待执行的函数从to_do_list_中移动到functors中，然后执行functors中的函数。
// 锁只保护数据交换，不保护执行
// 性能考量：最小量化临界区，避免执行任务时阻塞其他线程
// 安全防护：避免死锁，尤其避免递归添加新任务
    //A获取锁，并执行B，B可能需要获取锁（修改任务列表to_do_list_)，导致死锁
// 原子性保证：原子性保证，确保每轮处理的任务集是确定快照
// 系统稳定性：防止任务无限积累导致事件循环饥饿
void EventLoop::DoToDoList(){
    // 此时已经epoll_wait出来，可能存在阻塞在epoll_wait的可能性。
    calling_functors_ = true;

    std::vector < std::function<void()>> functors;
    {
        // 加锁 保证线程同步
        std::unique_lock<std::mutex> lock(mutex_); 
        functors.swap(to_do_list_);
    }
    for(const auto& func: functors){
        func();
    }

    calling_functors_ = false;
}

void EventLoop::HandleRead(){
    // 用于唤醒EventLoop
    uint64_t read_one_byte = 1;
    ssize_t read_size = ::read(wakeup_fd_, &read_one_byte, sizeof(read_one_byte));
    (void) read_size;
    assert(read_size == sizeof(read_one_byte));
    return;
}


void EventLoop::RunAt(TimeStamp timestamp, std::function<void()>const & cb){
    timer_queue_->AddTimer(timestamp, std::move(cb), 0.0);
}

void EventLoop::RunAfter(double wait_time, std::function<void()> const & cb){
    TimeStamp timestamp(TimeStamp::AddTime(TimeStamp::Now(), wait_time));
    timer_queue_->AddTimer(timestamp, std::move(cb), 0.0);
}

void EventLoop::RunEvery(double interval, std::function<void()> const & cb){
    TimeStamp timestamp(TimeStamp::AddTime(TimeStamp::Now(), interval));
    timer_queue_->AddTimer(timestamp, std::move(cb), interval);
}
