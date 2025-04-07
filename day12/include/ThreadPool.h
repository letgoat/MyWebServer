#pragma once

#include <future>
#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>

class ThreadPool{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    template<class F, class... Args>
    auto add(F&& f, Args&&... args)
    ->std::future<typename std::result_of<F(Args...)>::type>;

};

//不能放在cpp文件中，C++编译器不支持模版的分离编译
template<class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args) //函数签名
    ->std::future<typename std::result_of<F(Args...)>::type> //函数返回类型
{
    using return_type = typename std::result_of<F(Args...)>::type; //函数返回类型

    auto task = std::make_shared<std::packaged_task<return_type()> >(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    ); 

    std::future<return_type> res = task->get_future(); //获取任务的返回值
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);

        if(stop){ //如果线程池已经停止，则抛出异常
            throw std::runtime_error("enqueue on stopped ThreadPool");
        }

        tasks.emplace([task](){(*task)();}); //将任务放入队列

    }
    cv.notify_one();
    return res; //返回任务的返回值
}