//日志功能除了LogStream日志的输出流，还需要前端内容和后端处理，
//对于前端的内容，主要是提供给用户访问日志库的接口，并对日志信息进行一定的格式化，提供给用户将日志信息写进缓冲区的功能
//为了实现此功能，我们创建一个Logger类
//为了针对不同的日志等级进行不同的操作，首先要定义不同的日志等级，通常，日志等级包含如下几个部分：
//1. DEBUG指出细粒度信息事件对调试应用程序是非常有帮助的（开发过程中使用）
//2. INFO表明程序在粗粒度级别上突出强调应用程序的运行过程
//3. WARN系统能正常运行，但可能会存在潜在错误的情况
//4. ERROR指出虽然发生错误事件，但仍然不影响系统的继续运行
//5. FATAL指出每个严重的错误事件将会导致应用程序的退出


//同步日志库的实现流程：当用户使用一个LOG_*的日志宏时，会创建一个临时匿名对象Logger
//然后Logger中存在一个Impl对象，创建该对象时，会将当前的时间和线程等信息加入到Buffer中
//之后，该日志宏返回Impl的LogStream的引用，并将相应信息输入到其拥有的Buffer中
//当调用结束，Logger对象被析构时候，会调用g_output将日志信息输出。并且根据不同的日志等级进行不同的操作


//该文件中存在3个class：Logger、SourceFile、Impl; 其中Logger中包含SourceFile(public), Impl(private)


#pragma once

#include <string.h>

#include "LogStream.h"


class Logger{
public:
    //日志等级
    enum LogLevel{
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    //编辑器计算源文件名
    class SourceFile{
        public:
            SourceFile(const char *data);
            const char* data_;
            int size_;
    };

    //构造函数，主要用于构造Impl
    Logger(const char* file, int line, LogLevel level);
    Logger(const char* file, int line, LogLevel level, const char* func);
    ~Logger();


    //用于日志宏，返回Impl的输出流
    LogStream& stream();

    //全局方法，设置日志级别，flush输出到目的地
    static LogLevel loglevel();
    static void SetLogLevel(LogLevel level);

    typedef void (*OutputFunc)(const char* data, int len); //设置函数指针
    typedef void (*FlushFunc)();

    //默认fwrite到stdout
    static void SetOutput(OutputFunc);

    //默认fflush到stdout
    static void SetFlush(FlushFunc);

private:
    //私有类，对日志信息进行组装，不对外开放
    class Impl{
        public:
            DISALLOW_COPY_AND_ASSIGN(Impl);
            typedef Logger::LogLevel LogLevel;
            typedef Logger::SourceFile SourceFile;
            Impl(LogLevel level, const SourceFile& source, int line);
            void FormattedTime(); //格式化时间信息
            void Finish(); //完成格式化，并补充输出源文件和源码位置

            LogStream& stream();
            const char* loglevel() const; //获取LogLevel的字符串
            LogLevel level_; //日志等级

        private:
            SourceFile sourcefile_; //源代码名称
            int line_; //源代码行号
            LogStream stream_; //日志缓存流
    };
    Impl impl_; //日志组装类
};

//全局的日志级别，静态函数定义，静态函数实现
extern Logger::LogLevel g_loglevel;
inline Logger::LogLevel Logger::loglevel(){
    return g_loglevel;
}


//日志宏
#define LOG_DEBUG if(Logger::loglevel() <= Logger::DEBUG) \
    Logger(___FILE___, __LINE__, Logger::DEBUG, _func_).stream()

#define LOG_INFO if(Logger::loglevel() <= Logger::INFO) \
    Logger(__FILE__, __LINE__, Logger::INFO).stream()

#define LOG_WARN Logger(__FILE__, __LINE__, Logger::WARN).stream()

#define LOG_ERROR Logger(__FILE__, __LINE__, Logger::ERROR).stream()

#define LOG_FATAL Logger(__FILE__, __LINE__, Logger::FATAL).stream()