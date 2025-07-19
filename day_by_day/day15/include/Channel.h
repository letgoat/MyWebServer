//channel是网络库的核心组件之一，他对socket进行了封装，保存了我们需要监听的socket事件，以及当前socket准备好的事件并进行处理。
//此外为了获得epoll的状态，需要使用eventloop进行管理

#pragma once
#include "common.h"
#include <functional>
#include <memory>

class EventLoop;
//私有成员：fd_, loop_, listen_events_, ready_events_, in_epoll_, read_callback_, write_callback_
class Channel{
public:
    DISALLOW_COPY_AND_MOVE(Channel);
    Channel(int fd, EventLoop* loop);

    ~Channel();

    void HandleEvent() const; //处理事件
    void HandleEventWithGuard() const;
    void Tie(const std::shared_ptr<void>& ptr);
    void EnableRead(); //允许读
    void EnableWrite(); //允许写
    void EnableET(); //以ET形式触发
    void DisableWrite();

    int fd() const; //获取fd
    short listen_events() const; //监听的事件
    short ready_events() const; //准备好的事件

    bool IsInEpoll() const; //判断当前channel是否在epoller中
    void SetInEpoll(bool in = true); //设置当前状态为epoller中

    void SetReadyEvents(int ev); //设置准备好的事件
    void set_read_callback(std::function<void()> const &callback); //设置回调函数
    void set_write_callback(std::function<void()> const &callback);

private:
    int fd_;
    EventLoop *loop_;
    std::weak_ptr<void> tie;
    short listen_events_;
    short ready_events_;
    bool in_epoll_;
    std::function<void()> read_callback_;
    std::function<void()> write_callback_;
};