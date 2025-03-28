#include "../include/ThreadPool.h"

ThreadPool::ThreadPool(int size):stop(false){
    for(int i = 0; i < size; i++){
        threads.emplace_back(std::thread([this](){ //线程执行的是一个lambda表达式，该表达式捕获了当前对象this，以便可以访问线程池的成员变量
            while(true){
                std::function<void()> task; //定义一个函数对象task，可以存储从任务队列中取出的任务
                {
                    std::unique_lock<std::mutex> lock(task_mtx);
                    cv.wait(lock, [this](){
                        return stop || !tasks.empty();
                    });
                    if(stop && tasks.empty()) return;
                    task = task.front();
                    tasks.pop();
                }
                task();
            }
        }));
    }
}