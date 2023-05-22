cmake_minimum_required(VERSION 3.23)
project(OS_EX3 C)

set(CMAKE_C_STANDARD 11)

add_executable(OS_EX3
        main.c BoundedQueue.c BoundedQueue.h Producer.c Producer.h ControlAndData.c ControlAndData.h)
