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
    dataAllocation(1, sizeof(BoundedQueue), (void *) &boundedQueue);//todo: Release!
    // Allocate data for its articles array.
    dataAllocation(queueSize, sizeof(Article *), (void **) &boundedQueue->queueArticles);//todo: Release!
    // Set the size of the queue.
    boundedQueue->boundedQueueSize = queueSize;
    // Set the values of the insert location and consume location to 0.
    boundedQueue->insert = 0;
    boundedQueue->consume = 0;
    // Initiate the mutex.
    pthread_mutex_init(&boundedQueue->mutex, NULL); //todo: Release!
    sem_init(&boundedQueue->empty, 0, queueSize); //todo: Release!
    // Set the semaphore full to the queue size, to initiate it to all spots available.
    sem_init(&boundedQueue->full, 0, 0);//todo: Release!
    // Return the bounded queue.
    return boundedQueue;
}

/**
 * Create an article using the producerId, articlesType for its title,
 * and the articlesCount to track how many was made from this type.
 * @param producerId The ID of the producer who made the article.
 * @param articlesType The types of what the article can be (string).
 * @param articlesCount The count for the article (how many made so far).
 * @return A pointer to the article on the heap.
 */
Article *createArticle(int producerId, char *articlesType, int articleCount, int serial) {
    // Create a new article and allocate data for it.
    Article *article;
    dataAllocation(1, sizeof(Article), (void *) &article);
    // Assign the producer id to the article.
    article->madeByProducerID = producerId;
    // Assign the type (string) of the article.
    strcpy(article->articleStr, articlesType);
    // Assign the number of articles produces so far from this type (not included).
    article->lastNumOfArticles = articleCount;
    // Set the serial number of the article.
    article->serial = serial;
    return article;
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
 * @return boundedQueue The bounded queue to push it to.
 */
Article *popFromBoundedQueue(BoundedQueue *boundedQueue) {
    sem_wait(&boundedQueue->full);
    pthread_mutex_lock(&boundedQueue->mutex);
    Article *article = boundedQueue->queueArticles[boundedQueue->consume];
    boundedQueue->consume = (boundedQueue->consume + 1) % boundedQueue->boundedQueueSize;
    pthread_mutex_unlock(&boundedQueue->mutex);
    sem_post(&boundedQueue->empty);
    return article;
}