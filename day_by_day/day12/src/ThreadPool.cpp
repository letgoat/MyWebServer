#include "../include/ThreadPool.h"

ThreadPool::ThreadPool(int size) : stop(false){
    for(int i = 0; i < size; ++i){
        threads.emplace_back(std::thread([this](){
            while(true){
                std::function<void()> task; //包装器task
                {
                    std::unique_lock<std::mutex> lock(tasks_mtx);
                    cv.wait(lock, [this](){
                        return stop || !tasks.empty(); //线程阻塞，直到线程池停止或有任务
                    });

                    if(stop && tasks.empty()) return; //如果线程池停止且任务队列为空，退出线程池
                    task = tasks.front(); //从队列中取出任务
                    tasks.pop(); //从队列中删除已经取出的任务
                }
                task();
            }
        }));
    }
}

ThreadPool::~ThreadPool(){
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        stop = true;
    }
    cv.notify_all();
    for(auto& th: threads){
        if(th.joinable()){
            th.join();
        }
    }
}
