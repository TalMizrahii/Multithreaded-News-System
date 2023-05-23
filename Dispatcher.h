#ifndef OS_EX3_DISPATCHER_H
#define OS_EX3_DISPATCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "BoundedQueue.h"
#include "ControlAndData.h"

#define DONE (-1)
#define SPORTS 0
#define WEATHER 1
#define NEWS 2

typedef struct {
    BoundedQueue *boundedQueue;
    Article **unboundedQueue;
    int unboundedQueueSize;
    int articleSerial;
} CoEditor;

typedef struct {
    BoundedQueue **BoundedQueues;
    CoEditor *sports;
    CoEditor *news;
    CoEditor *weather;
    int numOfProducers;
    int totalArticlesAmount;

} Dispatcher;

CoEditor *createCoEditor(int serial, BoundedQueue *boundedQueue);

Dispatcher *createDispatcher(BoundedQueue **boundedQueues);

Dispatcher *createNewDispatcher(BoundedQueue **boundedQueues,
                                int coEditorQueueSize,
                                int totalArticlesAmount,
                                int numOfProducers);

void *dispatch(void *dispatchArg);

#endif //OS_EX3_DISPATCHER_H
