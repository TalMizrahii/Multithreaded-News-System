#include "Producer.h"

/**
 * Destructor for the producer.
 * @param producer A Pointer to the producer pointer.
 */
void destroyProducer(Producer *producer) {
    // Deallocate the producer's data.
    free(producer);
    // Set its pointer to null.
    producer = NULL;
}

/**
 * A destructor for the producerJobArgs struct. Not destroying the producer or the queue.
 * @param producerJobArgs A pointer to the producerJobArgs pointer.
 */
void destroyProducerJobArgs(ProducerJobArgs *producerJobArgs) {
    // Deallocate the producerJobArgs struct.
    free(producerJobArgs);
    // Set it to NULL.
    producerJobArgs = NULL;
}

/**
 * Creating new producer.
 * @param producerId The producer id.
 * @param numberOfArticles The number of articles it needs to create.
 * @param queueSize The bound for its queue size.
 * @return A pointer to the new producer on the heap.
 */
Producer *createProducer(int producerId, int numberOfArticles, int queueSize) {
    // Create a new producer and allocate data for it.
    Producer *producer;
    dataAllocation(1, sizeof(Producer), (void *) &producer);
    // Set its producer's id.
    producer->producerId = producerId;
    // Set the number of articles it should produce.
    producer->numberOfArticles = numberOfArticles;
    // Set it the bound for the queue size.
    producer->queueSize = queueSize;
    // Return a pointer to the producer on the heap.
    return producer;
}

/**
 * Push all producer's article to his bounded queue.
 * @param producer The producer's struct.
 * @param boundedQueue The bounded queue of the producer.
 */
void pushArticlesToBoundedQueue(Producer *producer, BoundedQueue *boundedQueue) {
    // An array contains types of news.
    char *articlesType[NUM_OF_ARTICLES_TYPE] = {"SPORTS", "WEATHER", "NEWS"};
    // An array to store how many articles of this type where already generated.
    int articlesCount[NUM_OF_ARTICLES_TYPE] = {0, 0, 0};
    // Declare a random number for the article.
    int randomNumber = -1;
    // Declare a new article pointer.
    Article *article;
    // Create an article and push it to the bounded queue.
    for (int i = 0; i < producer->numberOfArticles; i++) {
        // Generate random number modulo 3, what means it can be 0, 1 or 2.
        randomNumber = rand() % NUM_OF_ARTICLES_TYPE;
        // Raise the counter for the relevant type.
        articlesCount[randomNumber]++;
        // Create an article.
        article = createArticle(producer->producerId,
                                articlesType[randomNumber],
                                articlesCount[randomNumber],
                                randomNumber);
        // Push it to the queue.
        pushToBoundedQueue(article, boundedQueue);
    }
}

/**
 * The producer's job is to produce articles as the amount given in the configuration file,
 * and push them in the bounded queue.
 * @param producerJobArgs The struct containing the producer and it's bounded queue.
 * @return NULL.
 */
void *producerJob(void *producerJobArgs) {
    // Extract the arguments from the struct.
    ProducerJobArgs *args = (ProducerJobArgs *) producerJobArgs;
    Producer *producer = args->producer;
    BoundedQueue *boundedQueue = args->boundedQueue;
    // Push the articles to the array.
    pushArticlesToBoundedQueue(producer, boundedQueue);
    // Set the last article to the bounded queue to sign the dispatcher this producer finished.
    Article *article = createArticle(producer->producerId, "DONE", -1, -1);
    // Push it to the queue.
    pushToBoundedQueue(article, boundedQueue);
    // Destroy the producer.
    destroyProducer(producer);
    // Destroy the ProducerJobArgs.
    destroyProducerJobArgs(producerJobArgs);
    return NULL;
}
