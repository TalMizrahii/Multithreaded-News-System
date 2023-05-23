#ifndef OS_EX3_BOUNDEDQUEUE_H
#define OS_EX3_BOUNDEDQUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include "ControlAndData.h"


typedef struct {
    Article **queueArticles;
    int boundedQueueSize;
    int insert;
    int consume;
    pthread_mutex_t mutex;
    sem_t empty;
    sem_t full;
} BoundedQueue;

BoundedQueue *createBoundedQueue(int queueSize);

void destroyBoundedQueue(BoundedQueue *boundedQueue);


void pushToBoundedQueue(Article *article, BoundedQueue *boundedQueue);

Article *popFromBoundedQueue(BoundedQueue *boundedQueue);

#endif //OS_EX3_BOUNDEDQUEUE_H
