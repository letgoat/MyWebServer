#pragma once

#include "common.h"

class EventLoop;
class Channel{
public:
    DISALLOW_COPY_AND_MOVE(Channel);
    Channel(int fd, EventLoop* loop);

    ~Channel();

    void handleEvent() const; //处理事件
    void EnableRead();
    void EnableWrite();
    void EnableET();
    void DisableWrite();

    int fd() const;
    short listen_events() const;
    short ready_events() const;

    bool IsInEpoll() const;
    void SetInEpoll(bool in = true);

    void SetReadyEvents(int ev);
};