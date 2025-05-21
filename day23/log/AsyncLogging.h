//对于日志库的主要逻辑而言，其关键的工作在于：
//1. 如何往buffer A中写入日志信息
//2. 后端如何操作并将相应的信息写入磁盘文件
//在同步日志库中，我们设置的Logger的输出和刷新都是stdout,我们只要将其变为往Buffer A中添加数据即可
//为了实现这个目的，定义一个class AsyncLogging

#pragma once

#include "Logging.h"

#include "Latch.h"
#include <vector>

static const double BufferWriteTimeout = 3.0; //等待写入的时间
static const int64_t FileMaxSize = 1024 * 1024 * 1024; // 单个文件最大为1G

class AsyncLogging{
private:
    bool running_; //是否正在运行
    const char* filepath_;
    std::mutex mutex_;
    std::condition_variable cv_;
    Latch latch_;
    std::thread thread_;

    std::unique_ptr<LogStream::Buffer> current_; //当前的缓存
    std::unique_ptr<LogStream::Buffer> next_; //空闲的缓存
    std::vector<std::unique_ptr<LogStream::Buffer>> buffers_; //已满的缓冲区

public:
    AsyncLogging(const char* filepath = nullptr);
    ~AsyncLogging();

    void Stop();
    void Start();
    
    void Append(const char* data, int len);
    void Flush();
    void ThreadFunc();
};