cmake_minimum_required(VERSION 3.10)
project(WebServer)

include_directories(
    base
    tcp
    log
    http
    timer
)

set(CMAKE_CXX_FLAGS "-g -Wall -Werror -std=c++14")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/test/)

# 多线程
find_package(Threads REQUIRED)

# libjsoncpp
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

aux_source_directory(base SRC_LIST1)
aux_source_directory(tcp SRC_LIST2)
aux_source_directory(http SRC_LIST3)
aux_source_directory(timer SRC_LIST4)
aux_source_directory(log SRC_LIST5)

# 将所有模块源文件合并
set(COMMON_SOURCES 
    ${SRC_LIST1} 
    ${SRC_LIST2} 
    ${SRC_LIST3} 
    ${SRC_LIST4} 
    ${SRC_LIST5}
)

# 为每个测试文件创建单独的可执行目标
add_executable(webserver test/http_server.cpp ${COMMON_SOURCES})
add_executable(test_logstream test/test_logstream.cpp ${COMMON_SOURCES})
add_executable(echoserver test/echo_server.cpp ${COMMON_SOURCES})
add_executable(test_context test/test_httpcontext.cpp ${COMMON_SOURCES})

# 设置链接库
target_link_libraries(webserver 
    ${CMAKE_THREAD_LIBS_INIT} 
    ${JSONCPP_LIBRARIES}
)

target_link_libraries(test_logstream 
    ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(echoserver 
    ${CMAKE_THREAD_LIBS_INIT}
)

target_link_libraries(test_context 
    ${CMAKE_THREAD_LIBS_INIT} 
    ${JSONCPP_LIBRARIES}
)