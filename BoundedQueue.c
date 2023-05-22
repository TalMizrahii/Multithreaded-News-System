#include "BoundedQueue.h"


/**
 * Create a bounded queue on the heap.
 * @param queueSize The queue size of articles.
 * @return The pointer to the bounded queue on the heap.
 */
BoundedQueue *createBoundedQueue(int queueSize) {
    // Declare a new queue.
    BoundedQueue *boundedQueue;
    // Allocate data for it.
    dataAllocation(1, sizeof(BoundedQueue *), (void *) &boundedQueue);//todo: Release!
    // Allocate data for its articles array.
    dataAllocation(queueSize, sizeof(Article*), (void**)&boundedQueue->queueArticles);//todo: Release!
    // Set the values of the insert location and consume location to 0.
    boundedQueue->insert = 0;
    boundedQueue->consume = 0;
    // Initiate the mutex.
    pthread_mutex_init(&boundedQueue->mutex, NULL); //todo: Release!
    sem_init(&boundedQueue->empty, 0, queueSize); //todo: Release!
    // Set the semaphore full to the queue size, to initiate it to all spots available.
    sem_init(&boundedQueue->full, 0, 0);//todo: Release!

}