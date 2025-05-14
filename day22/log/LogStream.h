#include <string>
#include <iostream>
#include "common.h"

static const int FixedBufferSize = 4096;
static const int kMaxNumericSize = 48;

class FixedBuffer{
public:
    FixedBuffer();
    ~FixedBuffer();

    void append(const char *data, int len); // append data to buffer
    const char *data() const; // get data in buffer
    int length() const; // get length of data in buffer

    char *current() const; // get current position in buffer
    int avail() const; // get available space in buffer
    void add(int len); //

    void reset(); // reset buffer
    const char* end() const; // get end of buffer
    void clear(); //clear buffer

private:
    char data_[FixedBufferSize];
    char *cur_;
}; 

class LogStream{
public:
    DISALLOW_COPY_AND_ASSIGN(LogStream);
    typedef FixedBuffer Buffer;
    typedef LogStream Self;
    LogStream();
    ~LogStream();

    void append(const char* data, int len);
    const Buffer* buffer() const;
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
    void formatInteger(T v);
    Buffer buffer_;
}