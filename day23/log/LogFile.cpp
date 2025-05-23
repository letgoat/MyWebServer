#include "LogFile.h"

LogFile::LogFile(const char* filepath): 
    fp_(::fopen(filepath, "a+")), 
    written_bytes_(0), 
    lastwrite_(0){
        if(!fp_){
            std::string DefaultPath = 
        }
}