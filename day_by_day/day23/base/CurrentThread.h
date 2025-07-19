#pragma once

#include <pthread.h>
#include <stdio.h>
//#include <stdint.h>

namespace CurrentThread{
    //__thread声明的变量在每个线程中都有独立的实例，因此不用的线程可以拥有不同的t_cachedTid值，而不会相互干扰
    extern __thread int t_cachedTid;
    extern __thread char t_formattedTid[32];
    extern __thread int t_formattedTidLength;

    void CacheTid();

    pid_t gettid();

    inline int tid(){
        //__builtin_expect()是GCC编译器的内置函数，用于告诉编译器某个条件的预期执行情况
        //它有两个参数：第一个参数是要判断的条件；第二个参数是预期的结果（0表示假，1表示真）
        if(__builtin_expect(t_cachedTid == 0, 0)){
            CacheTid();
        }
        return t_cachedTid;
    }

    inline const char* tidString(){return t_formattedTid;} //返回当前线程ID的字符串形式
    inline int tidStringLength(){return t_formattedTidLength;} //返回当前线程ID的字符串长度
}