#pragma once

#include <sys/time.h>
#include <stdio.h>
#include <string>

const int kMicroSecond2Second = 1000*1000; //用于微秒和秒的转换

class TimeStamp{
public:
    TimeStamp(): micro_seconds_(0){};
    ~TimeStamp(){};

    explicit TimeStamp(int64_t micro_seconds): micro_seconds_(micro_seconds){};
    bool operator<(const TimeStamp& rhs) const{
        return micro_seconds_ < rhs.micro_seconds_;
    }

    bool operator==(const TimeStamp& rhs) const{
        return micro_seconds_ == rhs.micro_seconds_;
    }

    int64_t micro_seconds() const{
        return micro_seconds_;
    }

    //获取事件的字符串表达，用于日志库
    void ToFormattedString() const{

    }

    static TimeStamp Now();
    static TimeStamp AddTime(TimeStamp timestamp, double add_seconds);

private:
    int64_t micro_seconds_;
};