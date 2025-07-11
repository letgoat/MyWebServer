#pragma once

#include <string.h>
#include "../base/common.h"
#include "../timer/TimeStamp.h"
#include "LogStream.h"

class Logger{

    public:
        DISALLOW_COPY_AND_MOVE(Logger);
        enum LogLevel
        {
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };

        // 编译器计算源文件名
        class SourceFile{
            public:
                SourceFile(const char *data);
                const char *data_;
                int size_;
        };

        // 构造函数，主要是用于构造Impl
        Logger(const char *file_, int line, LogLevel level);
        ~Logger();


        // 用于日志宏，返回Impl的输出流
        LogStream &stream();

        // 全局方法，设置日志全局日志级别，flush输出目的地
        static LogLevel logLevel();
        static void setLogLevel(LogLevel level);

        typedef void (*OutputFunc)(const char *data, int len); // 定义函数指针
        typedef void (*FlushFunc)();
        // 默认fwrite到stdout
        static void setOutput(OutputFunc);
        // 默认fflush到stdout
        static void setFlush(FlushFunc);

    private:
        // 私有类，对日志信息进行组装，不对外开放
        class Impl{
            public:
                DISALLOW_COPY_AND_MOVE(Impl);
                typedef Logger::LogLevel LogLevel;
                Impl(Logger::LogLevel level, const SourceFile &source, int line);
                void FormattedTime();// 格式化时间信息
                void Finish();// 完成格式化，并补充输出源码文件和源码位置

                LogStream &stream();
                const char *loglevel() const;// 获取LogLevel的字符串
                LogLevel level_;// 日志级别

            private:
                Logger::SourceFile sourcefile_; // 源代码名称
                int line_;// 源代码行数
                LogStream stream_;// 日志缓存流
        };
        Impl impl_;
};

// 全局的日志级别，静态成员函数定义，静态成员函数实现
extern Logger::LogLevel g_logLevel;
inline Logger::LogLevel Logger::logLevel(){
    return g_logLevel;
}


// 日志宏
#undef LOG_DEBUG
#undef LOG_INFO
#undef LOG_WARN
#undef LOG_ERROR
#undef LOG_FATAL
#define LOG_DEBUG if (Logger::logLevel() <= Logger::DEBUG) \
    Logger(__FILE__, __LINE__, Logger::DEBUG).stream()
#define LOG_INFO if (Logger::logLevel() <= Logger::INFO) \
    Logger(__FILE__, __LINE__, Logger::INFO).stream()
#define LOG_WARN if (Logger::logLevel() <= Logger::WARN) \
    Logger(__FILE__, __LINE__, Logger::WARN).stream()
#define LOG_ERROR if (Logger::logLevel() <= Logger::ERROR) \
    Logger(__FILE__, __LINE__, Logger::ERROR).stream()
#define LOG_FATAL if (Logger::logLevel() <= Logger::FATAL) \
    Logger(__FILE__, __LINE__, Logger::FATAL).stream()

// 以下是编译器常用的一些宏：
// __FILE__: 当前源文件名的字符串
// __LINE__: 当前源文件行号
// __func__: 当前函数名的字符串
// __DATE__: 编译日期的字符串
// __TIME__：编译时间的字符串
// __TIMESTAMP__: 源文件最后修改时间
// _WIN32: 编译环境是否为windows
// __linux__: 编译环境是否为linux
// __APPLE__: 编译环境是否为苹果系统
// __GNUC__: 编译器是否为gcc
// __cpluscplus: C++标准版本
