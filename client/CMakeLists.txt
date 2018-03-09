cmake_minimum_required(VERSION 3.9)
project(client)

if(MINGW)
    link_libraries(ws2_32)
endif()
set(CMAKE_CXX_STANDARD 11)

add_executable(client main.cpp)