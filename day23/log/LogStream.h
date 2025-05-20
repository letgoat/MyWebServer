// to complish log everything all the time
//一个日志库需要完成的功能主要有：
//1. 多种日志级别
//2. 日志输出地为本地文件
//3. 支持日志文件rolling(按天，按大小), 简化日志归档
//4. 日志文件命名(进程名+创建日期+创建时间+机器名+进程id+后缀log)
//5. 日志消息格式固定(日期+时间+线程id+日志级别+源文件名和行号+日志信息)


//该文件中存在3个class: FixedBuffer, LogStream, Fmt
//1. FixedBuffer是一个定长的字符串数组，用于存储日志信息
//2. LogStream是一个日志流，用于向日志缓冲区写入日志信息
//3. Fmt是一个格式化类，用于将一个数值类型数据转换成一个长度不超过32位字符串对象Fmt

#include <string>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include "common.h"

static const int FixedBufferSize = 4096;
static const int kMaxNumericSize = 48;

//网络端的Buffer使用std::string非常方便，但是由于其内部是动态分配内存，在频繁操作的字符串中，需要进行内存的动态分配和释放，因此效率极低
//因此我们使用一个定长的字符串数组来存储日志信息，可直接开辟对应的内存空间用于存储信息，在日志库中，将其定义为FixedBuffer。
class FixedBuffer{
public:
    FixedBuffer();
    ~FixedBuffer();

    void append(const char *buf, int len); // append data to buffer
    const char *data() const; // get data in buffer
    int length() const; // get length of data in buffer

    char *current() const; // get current position in buffer
    int avail() const; // get available space in buffer
    void add(int len); // add len to current position

    void reset(); // reset buffer
    const char* end() const; // get end of buffer
    void clear(); //clear buffer

private:
    char data_[FixedBufferSize]; //data_数组名就等于数组的起始地址
    char *cur_;
}; 


//在日志系统中，并非所有的数据都是字符数据，因此为了进行类型转换，并且采用类似C++风格的stream <<风格
//我们定义一个LogStream类，重载 << 操作符，之所以不直接使用iostream，是因为格式化输出麻烦，而且其操作并不是原子化
class LogStream{
public:
    DISALLOW_COPY_AND_ASSIGN(LogStream);
    typedef FixedBuffer Buffer;
    typedef LogStream Self;
    LogStream();
    ~LogStream();

    void append(const char* data, int len);
    const Buffer& buffer() const;
    void resetBuffer();

    //对于所有非字符类型进行重载 << 操作符
    Self& operator <<(bool v);

    Self& operator <<(short num);
    Self& operator <<(unsigned short num);
    Self& operator <<(int num);
    Self& operator <<(unsigned int num);
    Self& operator <<(long num);
    Self& operator <<(unsigned long num);
    Self& operator <<(long long num);
    Self& operator <<(unsigned long long num);

    Self& operator <<(float num);
    Self& operator <<(double num);
    Self& operator <<(long double num);

    Self& operator <<(char v);
    Self& operator <<(const char* str);

    Self& operator <<(const std::string& str);

private:
    template <typename T>
    void formatInteger(T value);
    Buffer buffer_;
};

//一个模版函数，将类型T的数值转换为字符串，并将其写入到日志缓冲区，步骤如下：
//1. 先判断缓冲区的空间是否足够
//2. 通过取模运算，逐位转换成字符（逆序存储）
//3. 符号处理，如果数值为负，在末尾追加负号
//4. 反转字符串，得到正序字符串
//5. 更新缓冲区，记录写入的字符的数量
template <typename T>
void LogStream::formatInteger(T value){
    //1. 先判断缓冲区的空间是否足够
    if(buffer_.avail() >= kMaxNumericSize){
        char *buf = buffer_.current();
        char *new = buf;
        //2. 通过取模运算，逐位转换成字符（逆序存储）
        bool isnegative_flag = flase;
        if(value < 0){
            isnegative_flag = true;
            value = -value;
        }
        do{
            int remainder = value % 10;
            *(now++) = reminder + '0';
            value /= 10;
        }while(value != 0)
        //3. 符号处理，如果数值为负，在末尾追加负号
        if(isnegative_flag){
            *(now++) = '-';
        }
        //4. 反转字符串，得到正序字符串
        std::reverse(buf, now);
        //5. 更新缓冲区，记录写入的字符的数量（更新cur_指针）
        buffer_.add(now - buf);
    }
}

//LogStream本身不支持格式化，因此需要额外的定义一个不影响其状态的Fmt类
//将一个数值类型数据转换成一个长度不超过32位字符串对象Fmt,并重载了支持Fmt输出到LogStream的<<操作符
//在Fmt内部，调用snprintf函数，对数据进行了格式化
class Fmt{
public:
    template <typename T>
    Fmt(const char *fmt, T value);

    const char* data() const {return buf_;}

    int length() const {return length_;}

private:
    char buf_[32];
    int length_;
};

template <typename T>
Fmt::Fmt(const char *fmt, T value){
    //如果T是数值类型，则std::is_arithmetic<T>::value为true,否则为false
    static_assert(std::is_arithmetic<T>::value == true, "Must be arithmetic type");

    //snprintf函数的第一个参数是输出的字符串buf，第二个参数是buf的大小，第三个参数是格式化字符串，第四个参数是要格式化的值
    length_ = snprintf(buf_, sizeof(buf_), fmt, value);
    //snprintf函数返回值是写入buf的字符数量，如果返回值大于buf的大小，说明格式化后的字符串过长，则会导致缓冲区溢出

    assert(static_cast<size_t>(length_) < sizeof(buf_));
}

inline LogStream& operator <<(LogStream& s, const Fmt& fmt){
    s.append(fmt.data(), fmt.length());
    return s;
}