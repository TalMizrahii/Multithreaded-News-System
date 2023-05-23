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

    // Return a pointer to the heap of the screen manager.
    return screenManager;
}


void *screenManagerJob() {

}