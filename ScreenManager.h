#ifndef OS_EX3_SCREENMANAGER_H
#define OS_EX3_SCREENMANAGER_H

#include "BoundedQueue.h"
#include "ControlAndData.h"


typedef struct {
    BoundedQueue *boundedQueue;
} ScreenManager;

#endif //OS_EX3_SCREENMANAGER_H
