#include "Dispatcher.h"



/**
 * Creating a dispatcher. NOT assigning it co-editors.
 * @param boundedQueues The bounded queues that it will go over.
 * @return A pointer to The dispatcher on the heap.
 */
Dispatcher *createDispatcher(BoundedQueue **boundedQueues) {
    // Declare the dispatcher and allocate data for it.
    Dispatcher *dispatcher;
    dataAllocation(1, sizeof(Dispatcher), (void *) &dispatcher);
    // Set to it the bounded queues.
    dispatcher->BoundedQueues = boundedQueues;
    // Return the pointer to the heap.
    return dispatcher;
}

/**
 * Creating a dispatcher and assigning it co editors.
 * @param boundedQueues The bounded queues of the producers.
 * @param numOfProducers The number of producers in the system.
 * @return A pointer to the new dispatcher on the heap.
 */
Dispatcher *createNewDispatcher(BoundedQueue **boundedQueues, int numOfProducers) {
    // Declare a dispatcher.
    Dispatcher *dispatcher = createDispatcher(boundedQueues);
    // Assign it co editors.
    dispatcher->sports = createCoEditor(SPORTS, createUnBoundedQueue());
    dispatcher->weather = createCoEditor(WEATHER, createUnBoundedQueue());
    dispatcher->news = createCoEditor(NEWS, createUnBoundedQueue());
    // Set the number of producers (and bounded queues).
    dispatcher->numOfProducers = numOfProducers;
    return dispatcher;
}


/**
 * sorting the given article to the correct bounded queue of the co editor.
 * @param article The article to sort.
 * @param dispatcher The dispatcher of the program.
 * @param indexRR The index that the dispatcher is on in the producers queue.
 */
void setArticleToCoEditor(Article *article, Dispatcher *dispatcher, int indexRR) {
    switch (article->serial) {
        case SPORTS:
            pushToUnBoundedQueue(article, dispatcher->sports->unBoundedQueue);
            break;
        case WEATHER:
            pushToUnBoundedQueue(article, dispatcher->weather->unBoundedQueue);
            break;
        case NEWS:
            pushToUnBoundedQueue(article, dispatcher->news->unBoundedQueue);
            break;
        case DONE:
            for (int i = indexRR; i < dispatcher->numOfProducers; ++i) {
                dispatcher->BoundedQueues[i] = dispatcher->BoundedQueues[i + 1];
            }
            dispatcher->numOfProducers = dispatcher->numOfProducers - 1;
            // todo: Free producer!
            break;
    }
}

/**
 *  The dispatcher's job. responsible to get an article from the producer's queue,
 *  and sort them to the co editors unbounded queues.
 * @param dispatchArg The dispatcher struct.
 * @return NULL.
 */
void *dispatch(void *dispatchArg) {
    // Extract the dispatcher.
    Dispatcher *dispatcher = (Dispatcher *) dispatchArg;
    // Set an index for the round-robin algorithm
    int indexRR = 0;
    // An article to transfer.
    Article *article;
    // As long as there are articles int the making.
    while (TRUE) {
        // Pop an article from the bounded queue.
        article = popFromBoundedQueue(dispatcher->BoundedQueues[indexRR]);
        // Sort the article to the correct co editor's queue.
        setArticleToCoEditor(article, dispatcher, indexRR);
        if (!dispatcher->numOfProducers) {
            break;
        }
        // Set the circle round-robin index to the next location.
        indexRR = (indexRR + 1) % dispatcher->numOfProducers;
        // Decrease the amount of articles to sort by 1.
        dispatcher->totalArticlesAmount = dispatcher->totalArticlesAmount - 1;
    }
    // Create a new article to sign the co editors that the dispatcher finished.
    article = createArticle(-1, "DONE", -1, -1);
    // Set it to all co editors.
    pushToUnBoundedQueue(article, dispatcher->sports->unBoundedQueue);
    pushToUnBoundedQueue(article, dispatcher->weather->unBoundedQueue);
    pushToUnBoundedQueue(article, dispatcher->news->unBoundedQueue);

    printf("dispatcher finish\n"); // DELETE!!!!!!!!!
    return NULL;
}