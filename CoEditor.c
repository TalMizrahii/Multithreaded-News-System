#include "CoEditor.h"


/**
 * A destructor for the co editor.Not destroying the bounded queue with the screen manager.
 * @param coEditor A pointer to a co editor to destroy.
 */
void destroyCoEditor(CoEditor *coEditor) {
    // Destroy the co-editors unbounded queue.
    destroyUnboundedQueue(coEditor->unBoundedQueue);
    // Deallocate the co-editor.
    free(coEditor);
    // Set its pointer to NULL.
    coEditor = NULL;
}

/**
 * Creating a new co editor.
 * @param serial The serial of the co editor.
 * @param unBoundedQueue The unbounded queue of the co-editor.
 * @return a pointer to the co editor on the heap.
 */
CoEditor *createCoEditor(int serial, UnBoundedQueue *unBoundedQueue) {
    // Create a new co editor and allocate data for it.
    CoEditor *coEditor;
    dataAllocation(1, sizeof(CoEditor), (void *) &coEditor); // todo: release!
    // Set it an unbounded queue.
    coEditor->unBoundedQueue = unBoundedQueue;
    // Set to it the co editor's serial number.
    coEditor->articleSerial = serial;
    // Return a pointer to the co editor on the heap.
    return coEditor;
}

/**
 * The co editor's job. Mainly transferring an article from the unbounded queue with the dispatcher,
 * to the bounded queue with the screen manager.
 * @param coEditorArg The co editor's struct.
 * @return NULL.
 */
void *coEditorJob(void *coEditorArg) {
    // Extract the co-editor.
    CoEditor *coEditor = (CoEditor *) coEditorArg;
    // create a pointer to store the articles.
    Article *article;
    // As long as there are articles to edit.
    while (TRUE) {
        // Pop an article from the unbounded queue.
        article = popFromUnBoundedQueue(coEditor->unBoundedQueue);
        // If it's a "DONE" article.
        if (article->serial == DONE) {
            pushToBoundedQueue(article, coEditor->SMBoundedQueue);
            break;
        }
        usleep(TENTH_SEC);
        pushToBoundedQueue(article, coEditor->SMBoundedQueue);
    }
    // Destroy the co editor.
//    destroyCoEditor(coEditor);
    return NULL;
}