//线程池的实现
#pragma once

#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <future>
#include <functional>

class ThreadPool{
public:
    ThreadPool(int size = std::thread::hardware_concurrency());
    ~ThreadPool();

    //void add(std::function<void()>);
    template<class F, class... Args>
    auto add(F&& f, Args&&... args)
    ->std::future<typename std::result_of<F(Args...)>::type>;

private:
    std::vector<std::thread> threads; //线程池
    std::queue<std::function<void()>> tasks; //任务队列
    std::mutex tasks_mtx; //互斥锁
    std::condition_variable cv; //条件变量
    std::atomic<bool> stop{false}; //停止标志
};

//不能放在cpp文件，C++编译器不支持模板的分离编译
template<class F, class... Args>
auto ThreadPool::add(F&& f, Args&&... args)
->std::future<typename std::result_of<F(Args...)>::type>{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(tasks_mtx);
        if(stop_)
            throw std::runtime_error("enqueue on stopped ThreadPool");

        tasks.emplace([task](){(*task)();});
    }
    cv.notify_one();
    return res;
}