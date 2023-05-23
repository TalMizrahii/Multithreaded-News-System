#ifndef OS_EX3_SCREENMANAGER_H
#define OS_EX3_SCREENMANAGER_H

#include "BoundedQueue.h"
#include "ControlAndData.h"
#include "Dispatcher.h"


typedef struct {
    BoundedQueue *boundedQueue;
} ScreenManager;

void *screenManagerJob(void *screenManagerArg);

ScreenManager *createScreenManager(BoundedQueue *boundedQueue);

ScreenManager *createNewScreenManager(int queueSize, Dispatcher *dispatcher);

#endif //OS_EX3_SCREENMANAGER_H
