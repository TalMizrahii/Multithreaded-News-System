#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "ControlAndData/ControlAndData.h"
#include "Producer/Producer.h"
#include "BoundedQueue/BoundedQueue.h"
#include "Dispatcher/Dispatcher.h"
#include "CoEditor/CoEditor.h"
#include "ScreenManager/ScreenManager.h"

/**
 * Check if the array needs to be resized.
 * @param currentArrayMaxSize The current array size (might not be full of elements).
 */
void checkResizeProdArray(Producer **producers, int *currentArrayMaxSize, int numOfProducers) {
    // Check if the array is full.
    if (numOfProducers >= *currentArrayMaxSize) {
        // If it is, double its size.
        *currentArrayMaxSize *= DOUBLE;
        // Allocate more memory.
        producers = realloc(producers, *currentArrayMaxSize * sizeof(Producer *));
        // Check if the allocation succeeded.
        if (producers == NULL) {
            printf("Error in realloc\n");
            exit(ERROR);
        }
    }
}

/**
 * Reading the configuration file into the global variables.
 * creating the producers array and setting the value of numProducers and coEditorQueueSize.
 * @param confPath The path to the configuration file.
 * @return
 */
void readConf(char *confPath,
              Producer **producers,
              int *numOfProducers,
              int producersArrayMaxSize,
              int *coEditorQueueSize,
              int *totalArticlesAmount) {

    // Open the configuration file.
    FILE *ConfFile = openFile(confPath);
    // Declare a default values to the producer.
    int producerId, numberOfArticles, queueSize;
    // While there is something to read, read it to the producerId.
    while (fscanf(ConfFile, "%d", &producerId) == 1) {
        // If the line wasn't the last line, read it to the numberOfArticles.
        if (fscanf(ConfFile, "%d", &numberOfArticles) <= ERROR)
            // If it was, break and set it to the coEditorQueueSize.
            break;
        // Add the amount of articles to the total amount.
        *totalArticlesAmount = *totalArticlesAmount + numberOfArticles;
        // Read the bound for the produce's queue size.
        fscanf(ConfFile, "%d", &queueSize);
        // Ignore any extra characters on the line.
        fscanf(ConfFile, "%*[^\n]");
        // Set the values to the producer.
        producers[*numOfProducers] = createProducer(producerId, numberOfArticles, queueSize);
        // increase the amount of producers.
        *numOfProducers = *numOfProducers + 1;
        // Check for the need to resize the producers array.
        checkResizeProdArray(producers, &producersArrayMaxSize, *numOfProducers);
    }
    *totalArticlesAmount = *totalArticlesAmount + *numOfProducers;
    // If the loop ended, the producerId is actually the coEditorQueueSize.
    *coEditorQueueSize = producerId;
    // Close the configuration file.
    fclose(ConfFile);
    // Validate the configuration file.
    ValidateConfigurationFile(*numOfProducers, *coEditorQueueSize);
}

/**
 * Create Bounded queues for the producers.
 * @param producers The producers array to know what size each queue.
 * @param numOfProducers The number of producers (which is the number of the bounded queues).
 * @param boundedQueues The array of the bounded queues.
 */
void createBoundedQueues(Producer **producers, int numOfProducers, BoundedQueue **boundedQueues) {
    for (int i = 0; i < numOfProducers; i++) {
        // Create a bounded queue and set it to the array.
        boundedQueues[i] = createBoundedQueue(producers[i]->queueSize);
    }
}

/**
 * Launch all producers with their bounded buffer threw an ProducerJobArgs struct.
 * @param producers The array of producers.
 * @param numOfProducers The amount of producers.
 * @param boundedQueues The array of the bounded queue.
 */
void createProducersJob(pthread_t producersThreads[],
                        Producer **producers,
                        int numOfProducers,
                        BoundedQueue **boundedQueues,
                        ProducerJobArgs *producerJobArgs[]) {
    // Go over all producers.
    for (int i = 0; i < numOfProducers; i++) {
        // Create an array for the producers arguments.
        producerJobArgs[i] = (ProducerJobArgs *) malloc(sizeof(ProducerJobArgs));
        // Set to it the relevant producer.
        producerJobArgs[i]->producer = producers[i];
        // Set to it the bounded buffer.
        producerJobArgs[i]->boundedQueue = boundedQueues[i];
        // Create the thread and send it the job function.
        pthread_create(&producersThreads[i], NULL, producerJob, producerJobArgs[i]);
    }
}

/**
 * Launch the dispatcher.
 * @param dispatcherThread The tread of the dispatcher.
 * @param dispatcher The dispatcher struct.
 */
void createDispatcherJob(pthread_t *dispatcherThread, Dispatcher *dispatcher) {
    // Run the thread with the dispatch function so the dispatcher will process the bounded queues.
    pthread_create(dispatcherThread, NULL, dispatch, (void *) dispatcher);
}

/**
 * Launch the screen manager.
 * @param screenManagerThread The tread of the screen manager.
 * @param screenManager The screen manager struct.
 */
void createScreenManagerJob(pthread_t *screenManagerThread, ScreenManager *screenManager) {
    // Run the thread with the screenManagerJob function so the screenManager will process the bounded queue.
    pthread_create(screenManagerThread, NULL, screenManagerJob, (void *) screenManager);
}

/**
 * Launch the co-editors.
 * @param coEditorsThreads The co-editors threads array.
 * @param dispatcher The dispatcher which contains the co editors.
 */
void createCoEditorsJob(pthread_t coEditorsThreads[], Dispatcher *dispatcher) {
    // Create all threads of the co editors and assign them the dispatcher job.
    pthread_create(&coEditorsThreads[0], NULL, coEditorJob, (void *) dispatcher->sports);
    pthread_create(&coEditorsThreads[1], NULL, coEditorJob, (void *) dispatcher->weather);
    pthread_create(&coEditorsThreads[2], NULL, coEditorJob, (void *) dispatcher->news);
}

/**
 * Wait for all threads to finish.
 * @param producersThreads The producer's threads.
 * @param numOfProducers The amount of producers.
 * @param dispatcherThread The dispatcher thread.
 * @param coEditorsThreads The co editors threads.
 * @param screenManagerThread The screen manager threads.
 */
void finishThreads(pthread_t producersThreads[],
                   int numOfProducers,
                   const pthread_t *dispatcherThread,
                   pthread_t coEditorsThreads[],
                   const pthread_t *screenManagerThread) {

    // Wait for all producers threads to finish.
    for (int i = 0; i < numOfProducers; i++) {
        pthread_join(producersThreads[i], NULL);
    }
    // Wait for the dispatcher thread to finish.
    pthread_join(*dispatcherThread, NULL);

    // Wait for all co editors to finish.
    for (int i = 0; i < NUM_OF_ARTICLES_TYPE; i++) {
        pthread_join(coEditorsThreads[i], NULL);
    }
    // Wait for the screen manager thread to finish.
    pthread_join(*screenManagerThread, NULL);
}

/**
 * The main function. Responsible to manage as a control flow to process the configuration file,
 * initiate data and create threads.
 * @param argc The amount of arguments received to the program.
 * @param argv The array of arguments.
 * @return 0 if succeeded.
 */
int main(int argc, char *argv[]) {
    // Validate the number of arguments.
    argCheck(argc);
    // Seed the random number generator with the current time
    srand(time(NULL));
    // Set parameters about the program.
    int numOfProducers = 0, producersArrayMaxSize = 10, coEditorQueueSize = -1, totalArticlesAmount = 0;
    // Declare a pointer to the producers array.
    Producer **producers;
    // Allocate data for the producers array.
    dataAllocation(producersArrayMaxSize, sizeof(Producer *), (void **) &producers);
    // Read the configuration file.
    readConf(argv[1], producers, &numOfProducers, producersArrayMaxSize, &coEditorQueueSize, &totalArticlesAmount);
    // Create the Bounded queues for the producers.
    BoundedQueue **boundedQueues;
    // Allocate data for the bounded queue array.
    dataAllocation(numOfProducers, sizeof(BoundedQueue *), (void **) &boundedQueues);
    // Create the bounded queues.
    createBoundedQueues(producers, numOfProducers, boundedQueues);
    // Create an array for the producers arguments.
    ProducerJobArgs *producerJobArgs[numOfProducers];
    // Create the dispatcher.
    Dispatcher *dispatcher = createNewDispatcherAndCoEditors(boundedQueues, numOfProducers);
    // Create the screen manager.
    ScreenManager *screenManager = createNewScreenManager(coEditorQueueSize, dispatcher);
    // Create an array of threads for the producers.
    pthread_t producersThreads[numOfProducers];
    // Create an array of threads for the co-editos.
    pthread_t coEditorsThreads[NUM_OF_ARTICLES_TYPE];
    // Create the thread of the dispatcher.
    pthread_t dispatcherThread;
    // Create the thread of the screen manager.
    pthread_t screenManagerThread;
    //Launch the screen manager.
    createScreenManagerJob(&screenManagerThread, screenManager);
    // Launch the producers.
    createProducersJob(producersThreads, producers, numOfProducers, boundedQueues, producerJobArgs);
    // Launch the dispatcher.
    createDispatcherJob(&dispatcherThread, dispatcher);
    // Launch the co-editors.
    createCoEditorsJob(coEditorsThreads, dispatcher);
    // Finish all threads.
    finishThreads(producersThreads, numOfProducers, &dispatcherThread, coEditorsThreads, &screenManagerThread);
    return 0;
}