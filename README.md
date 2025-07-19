# MyWebServer

一个基于C++11/14开发的高性能Web服务器，采用Reactor模式和多线程架构，支持HTTP协议和文件服务功能。

## 🚀 项目特性

- **高性能架构**: 基于Epoll的I/O多路复用，Reactor模式设计
- **多线程支持**: 线程池处理并发连接
- **HTTP协议**: 完整的HTTP/1.1协议实现
- **文件服务**: 支持文件上传、下载、删除功能
- **日志系统**: 异步日志记录，支持多级别日志
- **JSON支持**: 集成JsonCpp库处理JSON数据
- **静态文件服务**: 支持HTML、图片等静态资源
```

## 🛠️ 构建要求

- **操作系统**: Linux (推荐Ubuntu 18.04+)
- **编译器**: GCC 7.0+ 或 Clang 5.0+
- **CMake**: 3.10+
- **依赖库**:
  - JsonCpp
  - pthread

## 🔧 构建步骤

1. **克隆项目**
   ```bash
   git clone https://github.com/letgoat/MyWebServer.git
   cd MyWebServer
   ```

2. **安装依赖**
   ```bash
   # Ubuntu/Debian
   sudo apt-get update
   sudo apt-get install build-essential cmake libjsoncpp-dev
   
   # CentOS/RHEL
   sudo yum install gcc-c++ cmake jsoncpp-devel
   ```

3. **编译项目**
   ```bash
   mkdir build
   cd build
   cmake ..
   make -j$(nproc)
   ```

4. **运行测试**
   ```bash
   # 运行HTTP服务器
   ./test/webserver
   
   # 运行Echo服务器
   ./test/echoserver
   
   # 运行日志测试
   ./test/test_logstream
   ```

## 📊 性能特性

- **并发处理**: 支持数千并发连接
- **内存管理**: 智能指针管理资源，避免内存泄漏
- **零拷贝**: 使用sendfile优化文件传输
- **异步日志**: 非阻塞日志记录，提高性能

## 🧪 测试

项目包含多个测试程序：

```bash
# 编译所有测试
make

# 运行HTTP服务器测试
./test/webserver

# 运行Echo服务器测试
./test/echoserver

# 运行日志测试
./test/test_logstream

# 运行HTTP上下文测试
./test/test_context
```

## 🤝 贡献

欢迎提交Issue和Pull Request！

1. Fork 项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 打开Pull Request


⭐ 如果这个项目对您有帮助，请给它一个星标！
