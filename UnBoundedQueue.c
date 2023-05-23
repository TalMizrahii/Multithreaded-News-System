#include "UnBoundedQueue.h"

/**
 * Destructor for the unbounded queue. NOT destroying the articles it pointed.
 * @param unBoundedQueue A pointer to the unbounded queue to destroy.
 */
void destroyUnboundedQueue(UnBoundedQueue *unBoundedQueue) {
    // Destroy the mutex when no longer needed.
    pthread_mutex_destroy(&unBoundedQueue->mutex);
    // Destroy the "empty" semaphore when no longer needed.
    sem_destroy(&unBoundedQueue->empty);
    // Free the array of POINTERS to articles. Not the articles!
    free(unBoundedQueue->queueArticles);
    // Set it to null.
    unBoundedQueue->queueArticles = NULL;
    // Free the unbounded queue.
    free(unBoundedQueue);
    // Set it to null.
    unBoundedQueue = NULL;
}


/**
 * Creating a new unbounded queue.
 * @return A pointer to the heap of the queue.
 */
UnBoundedQueue *createUnBoundedQueue() {
    // Allocate memory for the unbounded queue.
    UnBoundedQueue *unboundedQueue;
    dataAllocation(1, sizeof(UnBoundedQueue), (void *) &unboundedQueue);
    // Allocate memory for the queue articles.
    dataAllocation(INITIATED_QUEUE_VAL, sizeof(Article *),
                   (void **) &unboundedQueue->queueArticles);
    // Set the initiated value of the unbounded queue.
    unboundedQueue->unboundedQueueSize = INITIATED_QUEUE_VAL;
    // Set the "insert" index to 0.
    unboundedQueue->insert = 0;
    // Set the "consume" index to 0.
    unboundedQueue->consume = 0;
    // Initialize the mutex for thread synchronization.
    pthread_mutex_init(&unboundedQueue->mutex, NULL);
    // Initiate the semaphore of the "empty" status of the queue.
    sem_init(&unboundedQueue->empty, 0, 0);
    // Return the unbounded queue.
    return unboundedQueue;
}

/**
 * Check for the need to double the size of the unbounded queue.
 * @param unboundedQueue The unbounded queue.
 */
void resizeUnBoundedQueue(UnBoundedQueue *unboundedQueue) {
    // Check if there are elements as space.
    if (unboundedQueue->insert == unboundedQueue->unboundedQueueSize) {
        // If so, reallocate the size of the array to be double it's old size.
        unboundedQueue->unboundedQueueSize = unboundedQueue->unboundedQueueSize * DOUBLE;
        unboundedQueue->queueArticles = realloc(unboundedQueue->queueArticles,
                                                unboundedQueue->unboundedQueueSize * (sizeof(Article *)));
        // Check if the reallocation succeeded.
        if (unboundedQueue->queueArticles == NULL) {
            perror("Error in: realloc\n");
            exit(ERROR);
        }
    }
}

/**
 * Push an element to the unbounded queue safely.
 * @param article The article to push.
 * @param unboundedQueue The unbounded queue to push to.
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
    // raise the semaphore of indication on the amount of elements in the queue.
    sem_post(&unboundedQueue->empty);
}


/**
 * Pop an element from the unbounded queue safely.
 * @param unboundedQueue The unbounded queue to pop an element from.
 * @return The article that was popped.
 */
Article *popFromUnBoundedQueue(UnBoundedQueue *unboundedQueue) {
    // Check if there is an element in the queue we can pop.
    sem_wait(&unboundedQueue->empty);
    // Lock the critical section with a mutex.
    pthread_mutex_lock(&unboundedQueue->mutex);
    // Pop a new article from the queue.
    Article *article = unboundedQueue->queueArticles[unboundedQueue->consume];
    // Set the "consume" index to the next location.
    unboundedQueue->consume = unboundedQueue->consume + 1;
    // Unlock the critical section.
    pthread_mutex_unlock(&unboundedQueue->mutex);
    // Return the pointer to the article we popped.
    return article;
}
