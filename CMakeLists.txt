cmake_minimum_required(VERSION 3.23)
project(OS_EX3 C)

set(CMAKE_C_STANDARD 11)

add_executable(OS_EX3
        main.c BoundedQueue/BoundedQueue.c BoundedQueue/BoundedQueue.h Producer/Producer.c Producer/Producer.h ControlAndData/ControlAndData.c ControlAndData/ControlAndData.h Dispatcher/Dispatcher.c Dispatcher/Dispatcher.h UnboundedQueue/UnBoundedQueue.c UnboundedQueue/UnBoundedQueue.h ScreenManager/ScreenManager.c ScreenManager/ScreenManager.h CoEditor/CoEditor.c CoEditor/CoEditor.h)
