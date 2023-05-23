#include "Dispatcher.h"

/**
 * Creating a new co editor.
 * @param serial The serial of the co editor.
 * @param boundedQueue The Bounded queue of the co-editor.
 * @return a pointer to the co editor on the heap.
 */
CoEditor *createCoEditor(int serial, BoundedQueue *boundedQueue) {
    CoEditor *coEditor;
    dataAllocation(1, sizeof(CoEditor), (void *) &coEditor); // todo: release!
    coEditor->boundedQueue = boundedQueue;
    coEditor->articleSerial = serial;
    return coEditor;
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
 * @param coEditorQueueSize The co-editors bounded queue size.
 * @return A pointer to the new dispatcher on the heap.
 */
Dispatcher *createNewDispatcher(BoundedQueue **boundedQueues,
                                int coEditorQueueSize,
                                int totalArticlesAmount,
                                int numOfProducers) {
    // Declare a dispatcher.
    Dispatcher *dispatcher = createDispatcher(boundedQueues);
    // Assign it co editors.
    dispatcher->sports = createCoEditor(SPORTS, createBoundedQueue(coEditorQueueSize));
    dispatcher->weather = createCoEditor(WEATHER, createBoundedQueue(coEditorQueueSize));
    dispatcher->news = createCoEditor(NEWS, createBoundedQueue(coEditorQueueSize));
    // Set the total articles amount (to know when to stop).
    dispatcher->totalArticlesAmount = totalArticlesAmount;
    // Set the number of producers (and bounded queues).
    dispatcher->numOfProducers = numOfProducers;
    return dispatcher;
}

/**
 * sorting the given article to the correct bounded queue of the co editor.
 * @param article The article to sort.
 * @param dispatcher The dispatcher of the program.
 */
void setArticleToCoEditor(Article *article, Dispatcher *dispatcher, int indexRR) {
    switch (article->serial) {
        case SPORTS:
//            pushToBoundedQueue(article, dispatcher->sports->boundedQueue);
            break;
        case WEATHER:
//            pushToBoundedQueue(article, dispatcher->weather->boundedQueue);
            break;
        case NEWS:
//            pushToBoundedQueue(article, dispatcher->news->boundedQueue);
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
 *
 * @param dispatchArg
 * @return
 */
void *dispatch(void *dispatchArg) {
    // Extract the dispatcher.
    Dispatcher *dispatcher = (Dispatcher *) dispatchArg;
    // Set an index for the round-robin algorithm
    int indexRR = 0;
    // An article to transfer.
    Article *article;
    // As long as there are articles int the making.
    while (1) {
//        printf("totalArticlesAmount:%d\n", dispatcher->totalArticlesAmount);
//        printf("waiting for: %d\n", indexRR);
        // Pop an article from the bounded queue.
        article = popFromBoundedQueue(dispatcher->BoundedQueues[indexRR]);
        // Sort the article to the correct co editor's queue.
        setArticleToCoEditor(article, dispatcher, indexRR);
        if(!dispatcher->numOfProducers){
            break;
        }
        // Set the circle round-robin index to the next location.
        indexRR = (indexRR + 1) % dispatcher->numOfProducers;
        // Decrease the amount of articles to sort by 1.
        dispatcher->totalArticlesAmount = dispatcher->totalArticlesAmount - 1;
    }
    printf("dispatcher finish\n");
}