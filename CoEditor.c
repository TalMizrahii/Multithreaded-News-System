#include "CoEditor.h"
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
    Article *article;
    while (TRUE) {
        article = popFromUnBoundedQueue(coEditor->unBoundedQueue);
        if (article->serial == DONE) {
            // Push DONE
            break;
        }
        usleep(TENTH_SEC);
        // pushto screen manager.
    }
    return NULL;
}