#pragma once 
#include <sys/epoll.h>
#include <functional>

class EventLoop;
class Channel{
private:
    EventLoop *loop; //表示这个Channel所属的事件循环
    int fd; //表示这个Channel关注的I/O对象
    uint32_t events; //表示该通道感兴趣的时间类型
    uint32_t revents; //表示实际发生的事件类型
    bool inEpoll; //表示这个通道是否已经添加到epoll的监控集中
    std::function<void()> callback; //事件处理的回调函数，当感兴趣的事件发生时，这个回调函数会被调用以执行相应的处理逻辑
public:
    Channel(EventLoop* _loop, int _fd);
    ~Channel();

    void handleEvent();  //处理时间，调用回调函数
    void enableReading(); //启用读事件的监听，即告诉事件循环这个通道对读事件感兴趣

    int getFd();
    uint32_t getEvents();
    uint32_t getRevents();
    bool getInEpoll();
    void setInEpoll(); //设置通道已经在epoll集合中了
    void setRevents(uint32_t); //设置实际发生的事件类型
    void setCallback(std::function<void()>); //设置事件处理的回调函数
};