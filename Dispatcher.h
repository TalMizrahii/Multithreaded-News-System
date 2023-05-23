#ifndef OS_EX3_DISPATCHER_H
#define OS_EX3_DISPATCHER_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


#include "BoundedQueue.h"
#include "UnBoundedQueue.h"
#include "ControlAndData.h"
#include "CoEditor.h"






typedef struct {
    BoundedQueue **BoundedQueues;
    CoEditor *sports;
    CoEditor *news;
    CoEditor *weather;
    int numOfProducers;
    int totalArticlesAmount;

} Dispatcher;


Dispatcher *createDispatcher(BoundedQueue **boundedQueues);

Dispatcher *createNewDispatcherAndCoEditors(BoundedQueue **boundedQueues, int numOfProducers);

void *dispatch(void *dispatchArg);



#endif //OS_EX3_DISPATCHER_H
