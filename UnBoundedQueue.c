#include "UnBoundedQueue.h"

UnBoundedQueue *createUnBoundedQueue(int queueSize) {
    UnBoundedQueue *unboundedQueue;
    dataAllocation(1, sizeof(UnBoundedQueue), (void *) &unboundedQueue); // todo: Release!
    dataAllocation(queueSize, sizeof(Article *), (void **) &unboundedQueue->queueArticles); // todo: Release!
    unboundedQueue->unboundedQueueSize = queueSize;
    unboundedQueue->numElements = 0;
    unboundedQueue->insert = 0;
    unboundedQueue->consume = 0;
    pthread_mutex_init(&unboundedQueue->mutex, NULL); // todo: Release!
    sem_init(&unboundedQueue->empty, 0, queueSize); // todo: Release!
    return unboundedQueue;
}

void pushToUnBoundedQueue(Article *article, UnBoundedQueue *unboundedQueue) {
    sem_wait(&unboundedQueue->empty);
    pthread_mutex_lock(&unboundedQueue->mutex);
    unboundedQueue->queueArticles[unboundedQueue->insert] = article;
    unboundedQueue->insert = unboundedQueue->insert + 1;
    unboundedQueue->numElements++;
    pthread_mutex_unlock(&unboundedQueue->mutex);
}

Article *popFromUnBoundedQueue(UnBoundedQueue *unboundedQueue) {
    pthread_mutex_lock(&unboundedQueue->mutex);
    Article *article = unboundedQueue->queueArticles[unboundedQueue->consume];
    unboundedQueue->consume = (unboundedQueue->consume + 1) % unboundedQueue->unboundedQueueSize;
    unboundedQueue->numElements--;
    pthread_mutex_unlock(&unboundedQueue->mutex);
    sem_post(&unboundedQueue->empty);
    return article;
}
