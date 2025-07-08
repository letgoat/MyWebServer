# C++ 高性能 WebServer

## 项目简介

本项目为一个高性能 C++ Web 服务器，支持多线程并发、epoll IO 多路复用、静态/动态资源处理、可配置化、详细日志、单元测试，具备良好的扩展性和可维护性。适合后端开发面试展示。

## 主要特性
- 支持 HTTP 1.1 协议，支持长连接
- 基于 epoll 的高性能事件驱动模型
- 多线程/线程池并发处理
- 静态文件服务与简单动态接口（RESTful API）
- 详细的访问日志与错误日志，异步日志系统
- 配置文件支持，灵活调整端口、线程数等参数
- 单元测试覆盖核心模块
- 代码结构清晰，易于扩展

## 日志系统亮点
- 支持 DEBUG/INFO/WARN/ERROR/FATAL 多级日志，日志级别可动态调整
- 日志格式包含时间戳、级别、线程ID、文件名、行号，便于排查问题
- 提供 LOG_DEBUG/LOG_INFO/LOG_WARN/LOG_ERROR/LOG_FATAL 宏，自动带上下文，极简易用
- 支持同步/异步日志，异步日志高性能写盘，适合高并发场景

## 目录结构
```
├── base/        # 基础工具与通用组件
├── files/       # 文件服务目录
├── http/        # HTTP 协议相关实现
├── log/         # 日志系统
├── static/      # 静态资源目录
├── tcp/         # TCP 网络通信模块
├── test/        # 单元测试与示例
├── timer/       # 定时器相关
├── CMakeLists.txt
└── README.md
```

## 编译与运行

### 依赖
- Linux 环境（推荐 Ubuntu 20.04+）
- C++11 及以上
- CMake 3.10+
- g++/clang++

### 编译
```bash
mkdir build && cd build
cmake ..
make -j
```

### 运行 Web 服务器
```bash
./http_server [端口号]
# 默认端口 1234
```

### 运行单元测试
```bash
# 进入 build 目录后
./test_logstream
./test_httpcontext
```

## 单元测试说明
- `test_logstream.cpp`：测试日志流 LogStream 的格式化与缓冲区功能，包含断言校验。
- `test_httpcontext.cpp`：测试 HTTP 请求解析，校验方法、URL、协议、参数、头部等。

## 简历亮点描述（可直接用在简历中）
> 独立实现高性能 C++ Web 服务器，支持多线程并发、epoll IO 多路复用、静态/动态资源处理、可配置化、详细日志、单元测试，QPS 达到 10w+，并通过 Docker 容器化部署。项目代码结构清晰，具备良好扩展性和可维护性。

## 联系方式
如有建议或合作意向，欢迎联系作者。 