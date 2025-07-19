#include "Timer.h"

Timer::Timer(TimeStamp expirationTime, std::function<void()> &cb, bool interval):
    expirationTime_(expirationTime),
    cb_(cb),
    interval_(interval)
    {};

TimeStamp Timer::GetExpiration() const{
    return expirationTime_;
}

void Timer::ReStart(TimeStamp now){
    expirationTime_ = TimeStamp::AddTime(now, interval_);
}

void Timer::run() const{
    cb_();
}

bool Timer::GetIsrepeat() const{
    return isRepeat_;
}