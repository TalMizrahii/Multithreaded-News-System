#include "ScreenManager.h"

/**
 * Creating a screen manager.
 * @param boundedQueue A bounded queue for the screen manager.
 * @return A pointer to the screen manager.
 */
ScreenManager *createScreenManager(BoundedQueue *boundedQueue) {
    // Initiate a screen manager and allocate data for it.
    ScreenManager *screenManager;
    dataAllocation(1, sizeof(ScreenManager), (void *) &screenManager);
    // Set the bounded queue to the screen manager.
    screenManager->boundedQueue = boundedQueue;
    // Return a pointer to the heap of the screen manager.
    return screenManager;
}


/**
 *
 * @param queueSize
 * @param dispatcher
 * @return
 */
ScreenManager *createNewScreenManager(int queueSize, Dispatcher*dispatcher){
    // Create new bounded queue.
    BoundedQueue *boundedQueue = createBoundedQueue(queueSize);
    // Set the queue to the co editors.
    dispatcher->sports->SMBoundedQueue = boundedQueue;
    dispatcher->news->SMBoundedQueue = boundedQueue;
    dispatcher->weather->SMBoundedQueue = boundedQueue;
    // Create a new screen manager.
    ScreenManager *screenManager = createScreenManager(boundedQueue);
    return screenManager;
}



/**
 * Printing the article in the asked format.
 * @param article The article to print.
 */
void printArticle(const Article *article) {
    printf("Producer %d %s %d\n", article->madeByProducerID, article->articleStr, article->lastNumOfArticles);
}

/**
 *
 * @param screenManagerArg
 * @return
 */
void *screenManagerJob(void *screenManagerArg) {
    ScreenManager *screenManager = (ScreenManager *) screenManagerArg;
    int coEditorsLeft = NUM_OF_ARTICLES_TYPE;
    while (coEditorsLeft) {
        Article *article = popFromBoundedQueue(screenManager->boundedQueue);
        if (article->serial == DONE) {
            coEditorsLeft--;
            continue;
        }
        printArticle(article);
    }
    printf("DONE\n");
}