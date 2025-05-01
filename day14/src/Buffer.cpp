#include "../include/Buffer.h"

const std::string& Buffer::buf() const{
    return buf_;
}

const char* Buffer::c_str() const{
    return buf_.c_str();
}

void Buffer::set_buf(const char* buf){
    std::string new_buf(buf);
    buf_.swap(new_buf);
}

