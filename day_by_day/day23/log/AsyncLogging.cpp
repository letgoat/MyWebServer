#include "AsyncLogging.h"
#include <memory>
#include "LogFile.h"

AsyncLogging::AsyncLogging(const char* filepath):
    running_(false), filepath_(filepath), latch_(1){
        current_ = std::make_unique<LogStream::Buffer>();
        next_ = std::make_unique<LogStream::Buffer>();
}

AsyncLogging::~AsyncLogging(){
    if(running_){
        Stop();
    }
}

void AsyncLogging::Start(){
    running_ = true;

}

void AsyncLogging::Stop(){
    running_ = false;

    //唤醒后端线程
    cv_.notify_one();
    thread_.join();
}

void AsyncLogging::Flush(){
    fflush(stdout);
}

void AsyncLogging::Append(const char* data, int len){
    std::unique_lock<std::mutex> lock(mutex_);
    if(current_->avail() > len){
        current_->append(data, len);
    }
    else{
        //如果当前缓存没有空间，就将当前缓存放入到已满列表中
        buffers_.push_back(std::move(current_));
        if(next_){ //如果next_还是空闲的
            current_ = std::move(next_);
        }
        else{
            current_.reset(new LogStream::Buffer());
        }
        current_->append(data, len);
    }

    cv_.notify_one();
}


void AsyncLogging::ThreadFunc(){
    //创建成功，提醒主线程
    latch_.notify();

    std::unique_ptr<LogStream::Buffer> new_current_ = std::make_unique<LogStream::Buffer>();
    std::unique_ptr<LogStream::Buffer> new_next_ = std::make_unique<LogStream::Buffer>();

    std::unique_ptr<LogFile> logfile = std::make_unique<LogFile>();
}
