cmake_minimum_required(VERSION 3.9)
project(Server)

if(MINGW)
    link_libraries(ws2_32)
endif()
set(CMAKE_CXX_STANDARD 11)

add_executable(Server main.cpp)