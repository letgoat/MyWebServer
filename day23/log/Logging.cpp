//该文件存在1个新class: Template
//Template: 用于封装一个已知长度的字符串，方便LogStream处理


#include "Logging.h"

#include "CurrentThread.h"
#include "TimerStamp.h"

//为了实现多线程中日志时间格式化的效率，增加了两个__thread变量
//用于缓存当前线程存日期时间字符串，上一次日志记录的秒数

__thread char t_time[64]; //当前线程的时间字符串
__thread time_t t_lastsecond; //当前线程上一次日志记录的秒数

class Logger::SourceFile;

//方便一个已知长度读字符串被送入buffer中
class Template{
public:
    Template(const char* str, unsigned len):
        str_(str), len_(len){}
    const char* str_;
    const unsigned len_;
};

//重载运算符，可以方便LogStream处理Template类型的数据
inline LogStream& operator <<(LogStream& s, const Template& v){
    s.append(v.str_, v.len_);
    return s;
}

//重载运算符，可以方便LogStream处理SourceFile文件
inline LogStream& operator <<(LogStream& s, const Logger::SourceFile& v){
    s.append(v.data_, v.size_);
    return s;
}

//strlen的返回值类型为size_t
Logger::SourceFile::SourceFile(const char* data):data_(data),size_(static_cast<int>(strlen(data_))){
    const char* forward_flash = strrchr(data, '/');
    if(forward_flash){
        data_ = forward_flash + 1;
        size_ -= static_cast<int>(data_ - data);
    }
}

Logger::Impl::Impl(LogLevel level, const SourceFile& source, int line): 
    sourcefile_(source), level_(level), line_(line){

        FormattedTime();
        CurrentThread::tid();

        stream_ << Template(CurrentThread::tidString(), CurrentThread::tidStringLength());
        stream_ << Template(loglevel(), 6);
}

void Logger::Impl::FormattedTime(){
    //格式化输出时间
    TimeStamp now = TimeStamp::Now();
    //数据类型time_t是指纪元（Epoch，如 1970 年 1 月 1 日 00:00:00 UTC）以来的秒数
    time_t seconds = static_cast<time_t>(now.micro_seconds() / kMicroSecond2Second);
    int micro_seconds = static_cast<int>(now.micro_seconds() % kMicroSecond2Second);

    //变更日志记录的时间，如果不在同一秒，则更新时间

    if(t_lastsecond != seconds){
        struct tm tm_time;
        localtime_r(&seconds, &tm_time);
        snprintf(t_time, sizeof(t_time), "%4d%02d%02d %02d:%02d:%02d.",
                tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
        t_lastsecond = seconds;
    }
    Fmt us(".%06dZ", micro_seconds);
    stream_ << Template(t_time, 17) << Template(us.data(), 9);
}

//输出 源文件 和 行号
void Logger::Impl::Finish(){
    stream_ << " - " << sourcefile_.data_ << ":" << line_ << "\n";
}

LogStream& Logger::Impl::stream(){
    return stream_;
}

const char* Logger::Impl::loglevel() const{
    switch(level_){
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARN:
            return "WARN";
        case ERROR:
            return "ERROR";
        case FATAL:
            return "FATAL";
        default:
            return "UNKNOWN";
    }
    return nullptr;
}

//下面两个函数defaultOutput和defaultFlush不是成员函数

//默认输出函数
void defaultOutput(const char* msg, int len){
    fwrite(msg, 1, len, stdout); //默认输出到stdout: 将长度为len, 每个数据代销为1字节的msg输出到stdout流
}

//默认刷新函数
void defaultFlush(){
    fflush(stdout); //默认flush到stdout
}

//定义默认值
Logger::OutputFunc g_output = defaultOutput;
Logger::FlushFunc g_flush = defaultFlush;
Logger::LogLevel g_loglevel = Logger::INFO;

Logger::Logger(const char* file, int line, LogLevel level):
    impl_(level, SourceFile(file), line){
}

Logger::Logger(const char* file, int line, LogLevel level, const char* func):
    impl_(level, SourceFile(file), line){
}

Logger::~Logger(){
    //补足源码位置和行数
    impl_.Finish();
    //获取缓冲区
    const LogStream::Buffer& buf(stream().buffer());
    //默认输出到stdout
    g_output(buf.data(), buf.length());

    //当日志级别为FATAL时，flush设备缓冲区并终止程序
    if(impl_.level_ == FATAL){
        g_flush(); //刷新输出缓冲区，确保所有缓存的日志消息都被输出
        abort(); //终止程序
    }
}

LogStream& Logger::stream(){
    return impl_.stream();
}

void Logger::SetOutput(OutputFunc func){
    g_output = func;
}


void Logger::SetFlush(FlushFunc func){
    g_flush = func;
}

void Logger::SetLogLevel(LogLevel level){
    g_loglevel = level;
}

