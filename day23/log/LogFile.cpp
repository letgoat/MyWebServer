#include "LogFile.h"
#include "TimerStamp.h"
#include <iostream>
#include <string>


LogFile::LogFile(const char* filepath): 
    fp_(::fopen(filepath, "a+")), 
    written_bytes_(0), 
    lastwrite_(0){
        if(!fp_){
            std::string DefaultPath = std::move("../LogFiles/LogFile_" +
                              TimeStamp::Now().TimeStamp::ToFormattedDefaultLogString() +
                              ".log");
            fp_ = ::fopen(DefaultPath.data(), "a+");
        }
}

LogFile::~LogFile(){
    Flush();
    if(!fp_){
        ::fclose(fp_);
    }
}

void Write(const char *msg, int len){
    
}

void Flush(){

}

int64_t writtenbytes() const{

}