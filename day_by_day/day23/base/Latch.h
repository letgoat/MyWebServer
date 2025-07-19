#pragma once

#include "common.h"

#include <thread>
#include <condition_variable>

class Latch{
private:
    std::mutex mtx_;
    std::condition_variable cv_;
    int count_;

public:
    DISALLOW_COPY_AND_MOVE(Latch);
    
    explicit Latch(int count): count_(count){}
    
    void wait(){
        std::unique_lock<std::mutex> lock(mtx_);
        while(count_ > 0){
            cv_.wait(lock); //如果count_大于0，就一直在该线程阻塞等待
        }
    }

    void notify(){
        std::unique_lock<std::mutex> lock(mtx_);
        --count_;
        if(count_ == 0){
            cv_.notify_all(); //如果count_等于0，就通知所有等待的线程
        }
    }
};