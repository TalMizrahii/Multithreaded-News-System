#include "ScreenManager.h"

/**
 * A destructor for the screen manager.
 * @param screenManager A pointer to the screen manager.
 */
void destroyScreenManager(ScreenManager *screenManager) {
    // Destroy the bounded queue.
    destroyBoundedQueue(screenManager->boundedQueue);
    // Deallocate the memory of the screen manager struct.
    free(screenManager);
    // Set it to null.
    screenManager = NULL;
}

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
 * Creating a new screen manager from a dispatcher data.
 * @param queueSize The size of the bounded queue with the co editors.
 * @param dispatcher The dispatcher.
 * @return A pointer to the heap of the screen manager.
 */
ScreenManager *createNewScreenManager(int queueSize, Dispatcher *dispatcher) {
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
void printArticle(Article *article) {
    // Print the article data to the screen.
    printf("Producer %d %s %d\n", article->madeByProducerID, article->articleStr, article->lastNumOfArticles);
}


/**
 * The screen manager will pop an article that one of the co-editors pushed to the shared queue,
 * and then will print it to the screen.
 * @param screenManagerArg The screen manager pointer.
 * @return NULL.
 */
void *screenManagerJob(void *screenManagerArg) {
    // Extract the screen manager.
    ScreenManager *screenManager = (ScreenManager *) screenManagerArg;
    // Set a counter for the co editors amount.
    int coEditorsLeft = NUM_OF_ARTICLES_TYPE;
    // Declare two pointer to article. One for the popped article, one for the done article.
    Article *article, *doneArticle;
    // As long as the coEditors didn't finish.
    while (coEditorsLeft) {
        // Get an article from the queue.
        article = popFromBoundedQueue(screenManager->boundedQueue);
        // Check if it is the done queue.
        if (article->serial == DONE) {
            // save the done article (there is only one).
            doneArticle = article;
            // Decrease the amount of co editors left.
            coEditorsLeft--;
            continue;
        }
        // Print the article.
        printArticle(article);
        // destroy the article.
        destroyArticle(article);
    }
    // Destroy the done article.
    destroyArticle(doneArticle);
    // Print the DONE statement.
    printf("DONE\n");
    // Destroy the screen manager.
    destroyScreenManager(screenManager);
    return NULL;
}
