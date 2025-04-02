#include "../include/Buffer.h"
#include <iostream>

Buffer::Buffer(){

}

Buffer::~Buffer(){

}

void Buffer::append(const char* _str, int _size){
    for(int i = 0; i < _size; i++){
        if(_str[i] == '\0') break;
        buffer.push_back(_str[i]);
    }
}

ssize_t Buffer::size(){
    return buffer.size();
}

const char* Buffer::c_str(){
    return buffer.c_str();
}

void Buffer::clear(){
    buffer.clear();
}

void Buffer::getline(){
    buffer.clear();
    std::getline(std::cin, buffer);
}