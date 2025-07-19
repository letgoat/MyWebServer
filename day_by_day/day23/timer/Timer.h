#pragma once

#include <functional>
#include "TimerStamp.h"

class Timer{
public:
    Timer(TimeStamp expirationTime, std::function<void()> &cb, bool interval);

    //获取定时器的过期时间
    TimeStamp GetExpiration() const;

    //重新设置定时器的过期时间
    void ReStart(TimeStamp now); 

    //启动定时器
    void run() const;

    bool GetIsrepeat() const;
    
private:
    TimeStamp expirationTime_;
    std::function<void()> cb_;
    bool isRepeat_;
    double interval_;
};
