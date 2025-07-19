//该文件中存在1个class：TimeStamp
//TimeStamp: 当前时间点（单位微秒）
#pragma once

#include <sys/time.h>
#include <stdio.h>
#include <string>

const int kMicroSecond2Second = 1000*1000; //用于微秒和秒的转换

class TimeStamp{
private:
    int64_t micro_seconds_;

public:
    TimeStamp(): micro_seconds_(0){};
    explicit TimeStamp(int64_t micro_seconds): micro_seconds_(micro_seconds){};
    ~TimeStamp(){};

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
    std::string ToFormattedString() const{
        char buf[64] = 
    }

    static TimeStamp Now();
    static TimeStamp AddTime(TimeStamp timestamp, double add_seconds);

};

inline TimeStamp TimeStamp::Now(){
    //tiemval结构体包含__time_t tv_sec和__suseconds_t tv_usec两个成员变量，分别表示秒和微秒
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return TimeStamp(tv.tv_sec*kMicroSecond2Second + tv.tv_usec);
}

inline TimeStamp TimeStamp::AddTime(TimeStamp timestamp, double add_seconds){
    int64_t micro_seconds = static_cast<int64_t>(add_seconds*kMicroSecond2Second);
    return TimeStamp(timestamp.micro_seconds() + micro_seconds);
}