#include "BoundedQueue.h"

/**
 * Destructor for the bounded queue. NOT destroying the articles it pointed.
 * @param boundedQueue A pointer to the bounded queue to destroy.
 */
void destroyBoundedQueue(BoundedQueue *boundedQueue) {
    if (boundedQueue == NULL) return;
    // Destroy the mutex when no longer needed.
    pthread_mutex_destroy(&boundedQueue->mutex);
    // Destroy the semaphores when no longer needed
    sem_destroy(&boundedQueue->empty);
    sem_destroy(&boundedQueue->full);
    // Free the array of POINTERS to articles. Not the articles!
    free(boundedQueue->queueArticles);
    // Set it to null.
    boundedQueue->queueArticles = NULL;
    // Free the bounded queue.
    free(boundedQueue);
    // Set it to null.
    boundedQueue = NULL;
}


/**
 * Create a bounded queue on the heap.
 * @param queueSize The queue size of articles.
 * @return The pointer to the bounded queue on the heap.
 */
BoundedQueue *createBoundedQueue(int queueSize) {
    // Declare a new queue.
    BoundedQueue *boundedQueue;
    // Allocate data for it.
    dataAllocation(1, sizeof(BoundedQueue), (void *) &boundedQueue);
    // Allocate data for its articles array.
    dataAllocation(queueSize, sizeof(Article *), (void **) &boundedQueue->queueArticles);
    // Set the size of the queue.
    boundedQueue->boundedQueueSize = queueSize;
    // Set the values of the insert location and consume location to 0.
    boundedQueue->insert = 0;
    boundedQueue->consume = 0;
    // Initiate the mutex.
    pthread_mutex_init(&boundedQueue->mutex, NULL);
    sem_init(&boundedQueue->empty, 0, queueSize);
    // Set the semaphore full to the queue size, to initiate it to all spots available.
    sem_init(&boundedQueue->full, 0, 0);
    // Return the bounded queue.
    return boundedQueue;
}

/**
 * Pushing to a bounded queue an article. Making the push safe for the producer by using semaphores and mutexes.
 * @param article The article to push.
 * @param boundedQueue The bounded queue to push it to.
 */
void pushToBoundedQueue(Article *article, BoundedQueue *boundedQueue) {
    // Decrease the empty value by 1.
    sem_wait(&boundedQueue->empty);
    // Lock the mutex when entering the critical section.
    pthread_mutex_lock(&boundedQueue->mutex);
    // Set the article to its position.
    boundedQueue->queueArticles[boundedQueue->insert] = article;
    // Raise the insert location to the next position by 1 in a circle order.
    boundedQueue->insert = (boundedQueue->insert + NEXT_LOCATION) % boundedQueue->boundedQueueSize;
    // Unlock the mutex when leaving the critical section.
    pthread_mutex_unlock(&boundedQueue->mutex);
    // Raise the full counter.
    sem_post(&boundedQueue->full);
}

/**
 * Pop an article from a bounded buffer. Making the pop safe for the consumer by using semaphores and mutexes.
 * @param boundedQueue The bounded queue to pop from.
 * @return An article from the queue.
 */
Article *popFromBoundedQueue(BoundedQueue *boundedQueue) {
    // Check if the bounded queue is empty or not.
    sem_wait(&boundedQueue->full);
    // Lock the critical section.
    pthread_mutex_lock(&boundedQueue->mutex);
    // Get the article from the consume location.
    Article *article = boundedQueue->queueArticles[boundedQueue->consume];
    // Get the next consume index to the next location.
    boundedQueue->consume = (boundedQueue->consume + 1) % boundedQueue->boundedQueueSize;
    // Unlock the mutex.
    pthread_mutex_unlock(&boundedQueue->mutex);
    // Raise the empty semaphore by 1.
    sem_post(&boundedQueue->empty);
    // Return the article.
    return article;
}
