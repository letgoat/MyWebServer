#include "EventLoopThread.h"
#include "EventLoop.h"
#include <mutex>
#include <thread>
#include <condition_variable>

EventLoopThread::EventLoopThread() : loop_(nullptr){}

EventLoopThread::~EventLoopThread(){}

//主线程调用该函数
//创建子线程
//获取子线程创建的EventLoop对象指针
//立即返回该指针给调用者
EventLoop *EventLoopThread::StartLoop(){
    thread_ = std::thread(std::bind(&EventLoopThread::ThreadFunc, this));

    EventLoop *loop = nullptr;

    //loop_对于主线程和子线程是共享变量，因此需要mutex处理
    {
        std::unique_lock<std::mutex> lock(mutex_); 
        while (loop_ == NULL){
            cv_.wait(lock); // 当IO线程未创建LOOP时，阻塞
        }
        // 将IO线程创建的loop_赋给主线程。
        loop = loop_;
    
    }
    // 返回创建好的线程。
    return loop;
}


//创建EventLoop对象
//将对象指针赋值给loop_
//通知主线程初始化完成
void EventLoopThread::ThreadFunc(){
    // 由IO线程创建EventLoop对象
    EventLoop loop;
    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = &loop; // 获取子线程的地址
        cv_.notify_one(); // loop_被创建成功，发送通知，唤醒主线程。
    }

    loop_->Loop(); // 子线程主要处理函数，循环等待IO事件。
    {
        std::unique_lock<std::mutex> lock(mutex_);
        loop_ = nullptr;
    }
}
