#!/bin/bash

# WebServer 性能压测脚本
# 需先安装 wrk: sudo apt install wrk
# 启动 WebServer 后运行本脚本

THREADS=4         # 压测线程数
CONNECTIONS=100   # 并发连接数
DURATION=30s      # 压测时长
URL="http://127.0.0.1:1234/"  # 目标地址

if ! command -v wrk &> /dev/null; then
    echo "请先安装 wrk 工具 (sudo apt install wrk)"
    exit 1
fi

echo "=== WebServer 性能压测 ==="
echo "线程数: $THREADS, 连接数: $CONNECTIONS, 时长: $DURATION"
echo "目标: $URL"
wrk -t$THREADS -c$CONNECTIONS -d$DURATION $URL 