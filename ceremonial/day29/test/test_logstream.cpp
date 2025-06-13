#include "LogStream.h"
#include <iostream>
int main(){

    LogStream os;
    os << "hello world";
    std::cout << os.buffer().data() << std::endl;
    std::cout << "this is a blank1" << std::endl;
    os.resetBuffer();

    os << 11;
    std::cout << os.buffer().data() << std::endl;
    std::cout << "this is a blank2" << std::endl;
    os.resetBuffer();

    os << 0.1;
    std::cout << os.buffer().data() << std::endl;
    std::cout << "this is a blank3" << std::endl;
    os.resetBuffer();

    os << Fmt("%0.5f", 0.1);
    std::cout << os.buffer().data() << std::endl;
    std::cout << "this is a blank4" << std::endl;
    os.resetBuffer();

    return 0;
}