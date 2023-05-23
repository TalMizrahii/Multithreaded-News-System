#ifndef OS_EX3_UNBOUNDEDQUEUE_H
#define OS_EX3_UNBOUNDEDQUEUE_H

#include <pthread.h>
#include <semaphore.h>
#include <string.h>
#include "ControlAndData.h"

#define MAX_SIZE_OF_NEWS_STR 20

typedef struct {
    Article **queueArticles;
    int unboundedQueueSize;
    int numElements;
    int insert;
    int consume;
    pthread_mutex_t mutex;
    sem_t empty;
} UnBoundedQueue;

UnBoundedQueue *createUnBoundedQueue(int queueSize);
Article *createArticle(int producerId, char *articlesType, int articleCount, int serial);
void pushToUnBoundedQueue(Article *article, UnBoundedQueue *unboundedQueue);
Article *popFromUnBoundedQueue(UnBoundedQueue *unboundedQueue);

#endif //OS_EX3_UNBOUNDEDQUEUE_H
