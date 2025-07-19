#include <iostream>
#include "util.h"
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>


#define READ_BUFFER 1024
#define MAX_EVENTS 1024

void setnonblocking(int fd){
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL)|O_NONBLOCK);
}
//这个函数实际是设置socket为非阻塞模式，即在读写操作时，如果没有数据，则立即返回，而不是等待数据到来。
//int old_status = fcntl(fd, F_GETFL); 先获取当前socket的状态
//int new_status = old_status | O_NONBLOCK; 将原来的状态与O_NONBLOCK做或运算，得到新的状态
//fcntl(fd, F_SETFL, new_status) 设置新的状态

int main(){
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    errif(sockfd == -1, "create socket failed");

    struct sockaddr_in serv_addr;//创建服务器地址
    bzero(&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8888);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    errif(bind(sockfd, (sockaddr*)&serv_addr, sizeof(serv_addr)) == -1, "bind socket failed"); //sockfd绑定到服务器地址

    errif(listen(sockfd, SOMAXCONN) == -1, "socket listen error");//监听sockfd消息

    int epfd = epoll_create1(0);
    errif(epfd == -1, "epoll create failed");
    struct epoll_event events[MAX_EVENTS], ev; //创建epoll事件ev处理每个事件, 和事件数组events存储每个事件
    bzero(&events, sizeof(events));
    bzero(&ev, sizeof(ev));
    ev.data.fd = sockfd;
    ev.events = EPOLLIN | EPOLLET;//设置监听sockfd的读事件，并设置为边缘触发模式
    setnonblocking(sockfd);//设置sockfd为非阻塞模式
    epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &ev); //将sockfd添加到epoll中

    while(true){
        int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1); //等待epoll事件发生，超时时间为-1，即一直等待
        errif(nfds == -1, "epoll wait failed");
        for(int i = 0; i < nfds; i++){//开始处理事件
            if(events[i].data.fd == sockfd){ //新客户连接（新的客户端连接请求到达服务端的监听套接字sockfd）
                struct sockaddr_in clnt_addr; //创建新的socketaddr_in存储客户端的地址信息
                bzero(&clnt_addr, sizeof(clnt_addr));
                socklen_t clnt_addr_len = sizeof(clnt_addr);

                int clnt_sockfd = accept(sockfd, (sockaddr*)&clnt_addr, &clnt_addr_len);//接收客户端的连接请求，返回clnt_sockfd用于与客户端进行通信
                errif(clnt_sockfd == -1, "socket accept failed");
                printf("new client fd %d! IP: %s Port: %d\n", clnt_sockfd, inet_ntoa(clnt_addr.sin_addr), ntohs(clnt_addr.sin_port));

                bzero(&ev, sizeof(ev));
                ev.data.fd = clnt_sockfd;
                ev.events = EPOLLIN | EPOLLET;
                setnonblocking(clnt_sockfd);
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt_sockfd, &ev);                
            }
            else if(events[i].events & EPOLLIN){ //可读事件
                char buf[READ_BUFFER];
                while(true){ //由于使用非阻塞IO，读取客户端buffer,一次读取buffer大小数据，直到全部读取完毕
                    bzero(&buf, sizeof(buf));
                    ssize_t bytes_read = read(events[i].data.fd, buf, sizeof(buf));
                    if(bytes_read > 0){ //读取正常
                        printf("message from client fd: %d: %s\n", events[i].data.fd, buf);
                        write(events[i].data.fd, buf, bytes_read);//回显数据（客户端显示）
                    }
                    else if(bytes_read == -1){ //读取错误
                        if(errno == EINTR){//read过程被信号中断，继续读取
                            continue;
                        }
                        else if(errno == EAGAIN || errno == EWOULDBLOCK){ //当前无数据读，已经完全读完数据
                            printf("All data read from client fd: %d\n", events[i].data.fd);
                            break;
                        }
                        else{
                            perror("read error");
                            close(events[i].data.fd);
                            break;
                        }
                    }
                    else if(bytes_read == 0){//客户端关闭连接
                        printf("client fd: %d closed connection\n", events[i].data.fd);
                        close(events[i].data.fd);
                        break;
                    }
                   

                }
                
            }
            else{ //其他事件
                printf("other things happen\n");
            }
        }
    }
    close(sockfd);
    return 0;
}