#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#include "ControlAndData.h"
#include "Producer.h"
#include "BoundedQueue.h"

#define ARGUMENTS_NUM 2
#define ERROR (-1)
#define DOUBLE 2
#define CORRECTION 1
#define NUM_OF_ARTICLES_TYPE 3

typedef struct {
    Producer *producer;
    BoundedQueue *boundedQueue;
} ProducerJobArgs;

/**
 * Validate that the program received the proper amount of arguments.
 * @param argc The number of arguments.
 */
void argCheck(int argc) {
    if (argc != ARGUMENTS_NUM) {
        perror("Invalid argument number\n");
        exit(ERROR);
    }
}


/**
 * Opening the file from the configuration file.
 * @param file Pointer to the file type.
 * @param argv The path to the file to open.
 */
FILE *openFile(char *filePath) {
    // Open the file.
    FILE *file = fopen(filePath, "r");
    // If the file is null the opening fail. Print an error message and exit.
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(ERROR);
    }
    return file;
}


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
 * Check if the configuration file contains enough producers and the co-Editor queue size is positive.
 * @param numProducers the number of producers.
 * @param coEditorQueueSize The bound for the co editors queue size.
 */
void ValidateConfigurationFile(int numProducers, int coEditorQueueSize) {
    // Validate the number of producers received.
    if (numProducers <= 0) {
        perror("Not enough producers\n");
        exit(ERROR);
    }
    // Validate the size of the co-editors queue.
    if (coEditorQueueSize <= 0) {
        perror("Not A valid co-Editor queue size\n");
        exit(ERROR);
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
              int *coEditorQueueSize) {

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


void *producerJob(void *producerJobArgs) {
    // Extract the arguments from the struct.
    ProducerJobArgs *args = (ProducerJobArgs *) producerJobArgs;
    Producer *producer = args->producer;
    BoundedQueue *boundedQueue = args->boundedQueue;
    // An array contains types of news.
    char *articlesType[NUM_OF_ARTICLES_TYPE] = {"SPORTS", "WEATHER", "NEWS"};
    // An array to store how many articles of this type where already generated.
    int articlesCount[NUM_OF_ARTICLES_TYPE] = {0, 0, 0};
    // Declare a random number for the article.
    int randomNumber = -1;
    // Create an article and push it to the bounded queue.
    for (int i = 0; i < producer->numberOfArticles; i++) {
        // Generate random number modulo 3, what means it can be 0, 1 or 2.
        randomNumber = rand() % NUM_OF_ARTICLES_TYPE;
        // Raise the counter for the relevant type.
        articlesCount[randomNumber]++;
        // Create an article.
        Article *article = createArticle(producer->producerId, articlesType[randomNumber], articlesCount[randomNumber]);
        // Push it to the queue.
        pushToBoundedQueue(article, boundedQueue);
    }

    return NULL;
}

void createProducers(Producer **producers, int numOfProducers, BoundedQueue **boundedQueues) {
    // Create an array of threads.
    pthread_t threads[numOfProducers];
    // Create an array for the producers arguments.
    ProducerJobArgs *producerJobArgs[numOfProducers];
    // Go over all producers.
    for (int i = 0; i < numOfProducers; i++) {
        // Create an array for the producers arguments.
        producerJobArgs[i] = (ProducerJobArgs *) malloc(sizeof(ProducerJobArgs));
        // Set to it the relevant producer.
        producerJobArgs[i]->producer = producers[i];
        // Set to it the bounded buffer.
        producerJobArgs[i]->boundedQueue = boundedQueues[i];
        // Create the thread and send it the job function.
        pthread_create(&threads[i], NULL, producerJob, producerJobArgs[i]);
    }

    // Wait for all threads to finish
    for (int i = 0; i < numOfProducers; i++) {
        pthread_join(threads[i], NULL);
        printf("%d finished\n", i + 1);
    }

    // Release all producer args.
    for (int i = 0; i < numOfProducers; i++) {
        free(producerJobArgs[i]);
    }
}


/**
 *
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char *argv[]) {
    // Validate the number of arguments.
    argCheck(argc);
    // Set the number of producers to 0.
    int numOfProducers = 0, producersArrayMaxSize = 10, coEditorQueueSize = -1;
    // Declare a pointer to the producers array.
    Producer **producers;
    // Allocate data for the producers array.
    dataAllocation(producersArrayMaxSize, sizeof(Producer *), (void **) &producers);//todo: Release!
    // Read the configuration file.
    readConf(argv[1], producers, &numOfProducers, producersArrayMaxSize, &coEditorQueueSize);


    // DELETE from here!!!!
    // Printing the read values for verification
//    for (int i = 0; i < numOfProducers; i++) {
//        printf("Producer %d:\n", producers[i]->producerId);
//        printf("Producer value: %d\n", producers[i]->numberOfArticles);
//        printf("Queue size: %d\n\n", producers[i]->queueSize);
//    }
//    printf("Co-Editor queue size: %d\n", coEditorQueueSize);


    // Create the Bounded queues for the producers.
    BoundedQueue **boundedQueues;
    // Initiate the bounded queue array.
    dataAllocation(numOfProducers, sizeof(BoundedQueue *), (void **) &boundedQueues);//todo: Release!
    createBoundedQueues(producers, numOfProducers, boundedQueues);
    createProducers(producers, numOfProducers, boundedQueues);
    // Seed the random number generator with the current time
    srand(time(NULL));


    return 0;
}