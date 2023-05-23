#ifndef OS_EX3_SCREENMANAGER_H
#define OS_EX3_SCREENMANAGER_H

#include "../BoundedQueue/BoundedQueue.h"
#include "../ControlAndData/ControlAndData.h"
#include "../Dispatcher/Dispatcher.h"

typedef struct {
    BoundedQueue *boundedQueue;
} ScreenManager;

void *screenManagerJob(void *screenManagerArg);

void destroyScreenManager(ScreenManager *screenManager);

ScreenManager *createScreenManager(BoundedQueue *boundedQueue);

ScreenManager *createNewScreenManager(int queueSize, Dispatcher *dispatcher);

#endif //OS_EX3_SCREENMANAGER_H
