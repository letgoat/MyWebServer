//定义一个LogFile类，用于处理日志文件的写入和刷新操作

#pragma once

#include <iostream>

//用于表示日志文件内容被刷新到磁盘的时间间隔
static const time_t FlushInterval = 3;

class LogFile{
private:
    FILE *fp_; //指向日志文件的文件指针
    int64_t written_bytes_; //记录上次写入文件的字节数
    time_t lastwrite_; //记录上次写入日志的时间
    time_t lastflush_; //记录上次刷新日志的时间

public:
    LogFile(const char *filepath = nullptr);
    ~LogFile();

    //用于将指定的字符串写入日志文件
    void Write(const char *msg, int len);

    //用于将缓存中的内容强制刷新到磁盘
    void Flush();

    //返回已经写入日志文件的字节数
    int64_t writtenbytes() const;
};