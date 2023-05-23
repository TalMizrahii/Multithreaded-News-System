#include "UnBoundedQueue.h"


/**
 *
 * @param queueSize
 * @return
 */
UnBoundedQueue *createUnBoundedQueue() {
    UnBoundedQueue *unboundedQueue;
    dataAllocation(1, sizeof(UnBoundedQueue), (void *) &unboundedQueue); // Allocate memory for the unbounded queue
    dataAllocation(INITIATED_QUEUE_VAL, sizeof(Article *),
                   (void **) &unboundedQueue->queueArticles); // Allocate memory for the queue articles
    unboundedQueue->unboundedQueueSize = INITIATED_QUEUE_VAL;
    unboundedQueue->insert = 0;
    unboundedQueue->consume = 0;
    pthread_mutex_init(&unboundedQueue->mutex, NULL); // Initialize the mutex for thread synchronization
    sem_init(&unboundedQueue->empty, 0, 0);
    return unboundedQueue;
}

/**
 *
 * @param unboundedQueue
 * @param newQueueSize
 */
void resizeUnBoundedQueue(UnBoundedQueue *unboundedQueue) {
    if (unboundedQueue->insert == unboundedQueue->unboundedQueueSize) {
        unboundedQueue->unboundedQueueSize = unboundedQueue->unboundedQueueSize * DOUBLE;
        unboundedQueue->queueArticles = realloc(unboundedQueue->queueArticles,
                                                unboundedQueue->unboundedQueueSize * (sizeof(Article *)));
        if (unboundedQueue->queueArticles == NULL) {
            perror("Error in: realloc\n");
            exit(ERROR);
        }
    }
}

/**
 *
 * @param article
 * @param unboundedQueue
 */
void pushToUnBoundedQueue(Article *article, UnBoundedQueue *unboundedQueue) {
    // Lock the mutex when entering the critical section.
    pthread_mutex_lock(&unboundedQueue->mutex);
    // Set the article to its position.
    unboundedQueue->queueArticles[unboundedQueue->insert] = article;
    // Raise the insert location to the next position by 1 in a circle order.
    unboundedQueue->insert = unboundedQueue->insert + NEXT_LOCATION;
    // Check for the need to resize the queue.
    resizeUnBoundedQueue(unboundedQueue);
    // Unlock the mutex when leaving the critical section.
    pthread_mutex_unlock(&unboundedQueue->mutex);
    sem_post(&unboundedQueue->empty);

}


/**
 *
 * @param unboundedQueue
 * @return
 */
Article *popFromUnBoundedQueue(UnBoundedQueue *unboundedQueue) {
    sem_wait(&unboundedQueue->empty);
    pthread_mutex_lock(&unboundedQueue->mutex);
    Article *article = unboundedQueue->queueArticles[unboundedQueue->consume];
    unboundedQueue->consume = unboundedQueue->consume + 1;
    pthread_mutex_unlock(&unboundedQueue->mutex);


    return article;
}
