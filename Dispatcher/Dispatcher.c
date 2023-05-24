#include "Dispatcher.h"

/**
 * A destructor for the dispatcher. NOT destroying the articles and the co editors.
 * @param dispatcher A pointer to the dispatcher.
 */
void destroyDispatcher(Dispatcher *dispatcher) {
    // Free the all array of producers.
    free(dispatcher->BoundedQueues);
    // Set the full array to null.
    dispatcher->BoundedQueues = NULL;
    // Free the dispatcher.
    free(dispatcher);
    // Set it to null.
    dispatcher = NULL;
}

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
Dispatcher *createNewDispatcherAndCoEditors(BoundedQueue **boundedQueues, int numOfProducers) {
    // Declare a dispatcher.
    Dispatcher *dispatcher = createDispatcher(boundedQueues);
    // Assign it co editors.
    dispatcher->sports = createCoEditor(SPORTS, createUnBoundedQueue());
    dispatcher->weather = createCoEditor(WEATHER, createUnBoundedQueue());
    dispatcher->news = createCoEditor(NEWS, createUnBoundedQueue());
    // Set the number of producers (and bounded queues).
    dispatcher->currentNumOfProducers = numOfProducers;
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
    // Check what queue of co editor the article belong.
    switch (article->serial) {
        case SPORTS:
            // If it is a sports article, put it in it's co editor's queue.
            pushToUnBoundedQueue(article, dispatcher->sports->unBoundedQueue);
            break;
        case WEATHER:
            // If it is a weather article, put it in it's co editor's queue.
            pushToUnBoundedQueue(article, dispatcher->weather->unBoundedQueue);
            break;
        case NEWS:
            // If it is a news article, put it in it's co editor's queue.
            pushToUnBoundedQueue(article, dispatcher->news->unBoundedQueue);
            break;
        case DONE:
            // Free the ended queue.
            destroyBoundedQueue(dispatcher->BoundedQueues[indexRR]);
            // Destroy the done article.
            destroyArticle(article);
            // If it is the done article, shrink the producers array to the left.
            for (int i = indexRR; i < dispatcher->currentNumOfProducers - 1; ++i) {
                dispatcher->BoundedQueues[i] = dispatcher->BoundedQueues[i + 1];
            }
            // Decrease the number of producers by 1.
            dispatcher->currentNumOfProducers = dispatcher->currentNumOfProducers - 1;
            break;
    }
}

/**
 * The dispatcher's job. responsible to get an article from the producer's queue,
 * and sort them to the co editors unbounded queues.
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
        if (!dispatcher->currentNumOfProducers) {
            break;
        }
        // Set the circle round-robin index to the next location.
        indexRR = (indexRR + 1) % dispatcher->currentNumOfProducers;
    }
    // Create a new article to sign the co editors that the dispatcher finished.
    article = createArticle(-1, "DONE", -1, -1);
    // Set it to all co editors.
    pushToUnBoundedQueue(article, dispatcher->sports->unBoundedQueue);
    pushToUnBoundedQueue(article, dispatcher->weather->unBoundedQueue);
    pushToUnBoundedQueue(article, dispatcher->news->unBoundedQueue);
    // Destroy the dispatcher.
    destroyDispatcher(dispatcher);
    return NULL;
}
