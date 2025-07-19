cmake_minimum_required(VERSION 3.10)

project(Mywebserver)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(include)

add_executable(server
    src/server.cpp
    src/util.cpp
    )
add_executable(client
    src/client.cpp
    src/util.cpp)



    