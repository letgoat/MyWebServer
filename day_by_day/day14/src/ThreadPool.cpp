#include "../include/ThreadPool.h"
ThreadPool::ThreadPool(int size): stop(false){ 
    for(int i = 0; i < size; i++){ //创建size个工作线程
        threads.emplace_back([this](){
            while(true){
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    cv.wait(lock, [this](){ //释放锁并等待，直到线程池停止或者tasks非空
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) return;
                    task = tasks.front();
                    tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all(); //
    for(std::thread& th: threads){
        if(th.joinable()){
            th.join();
        }
    }
}