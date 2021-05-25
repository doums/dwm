cmake_minimum_required(VERSION 3.20)
project(xrootwin C)

set(CMAKE_C_STANDARD 11)

add_executable(xrootwin main.c)
target_link_libraries(xrootwin X11)
