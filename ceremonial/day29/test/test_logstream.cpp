#include "../log/LogStream.h"
#include <cassert>

int main(){

    LogStream os;
    os << "hello world";
    assert(std::string(os.buffer().data(), os.buffer().len()) == "hello world");
    os.resetBuffer();

    os << 11;
    assert(std::string(os.buffer().data(), os.buffer().len()) == "11");
    os.resetBuffer();

    os << 0.1;
    assert(std::string(os.buffer().data(), os.buffer().len()).find("0.1") != std::string::npos);
    os.resetBuffer();

    os << Fmt("%0.5f", 0.1);
    assert(std::string(os.buffer().data(), os.buffer().len()).find("0.10000") != std::string::npos);
    os.resetBuffer();

    return 0;
}