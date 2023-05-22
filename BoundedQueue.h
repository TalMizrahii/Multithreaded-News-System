#ifndef OS_EX3_BOUNDEDQUEUE_H
#define OS_EX3_BOUNDEDQUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include "ControlAndData.h"

#define MAX_SIZE_OF_NEWS_STR 20

typedef struct {
    char articleStr[MAX_SIZE_OF_NEWS_STR];
    int madeByProducerID;
    int articleType;
    int lastNumOfArticles;
} Article;


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

#endif //OS_EX3_BOUNDEDQUEUE_H
