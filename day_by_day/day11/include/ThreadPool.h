#pragma once

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <future>


class ThreadPool{
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex tasks_mtx;
    std::condition_variable cv;
    bool stop;
public:
    ThreadPool(int size = 10);
    ~ThreadPool();

    template<class F, class... Args>
    auto add(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>;
};

