#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>

#define ARGUMENTS_NUM 2
#define ERROR (-1)
#define INIT_PROD_AMOUNT 1
#define DOUBLE 2
#define NUM_OF_ARTICLES_TYPE 3
#define MAX_SIZE_OF_NEWS_STR 20
#define CORRECTION 1


typedef struct {
    char articleStr[MAX_SIZE_OF_NEWS_STR];
    int madeByProducerID;
    int articleType;
    int lastNumOfArticles;
} Article;


typedef struct {
    Article **queueArticles;
    int boundedQueueSize;
    int insert;
    int consume;
    pthread_mutex_t mutex;
    sem_t empty;
    sem_t full;
} BoundedQueue;


// Define the producer as an id, the number of articles he has to produce, and the size of his bounded queue size.
typedef struct {
    int producerId;
    int numberOfArticles;
    int queueSize;
    BoundedQueue boundedQueue;
} Producer;


// The array of producers.
Producer *producers;
// The number of producers in the system.
int numProducers;
// The co-editors bounded queue size. Initiate to -1.
int coEditorQueueSize = -1;


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
 * Check if the configuration file contains enough producers and the co-Editor queue size is positive.
 */
void ValidateConfigurationFile() {
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
void checkResizeProdArray(int *currentArrayMaxSize) {
    // Check if the array is full.
    if (numProducers >= *currentArrayMaxSize) {
        // If it is, double its size.
        *currentArrayMaxSize *= DOUBLE;
        // Allocate more memory.
        producers = realloc(producers, *currentArrayMaxSize * sizeof(Producer));
        // Check if the allocation succeeded.
        if (producers == NULL) {
            printf("Error in realloc\n");
            exit(ERROR);
        }
    }
}

/**
 * Setting the values to the current producer.
 * @param currentArrayMaxSize The current array size (might not be full of elements).
 * @param producerId The new producer id.
 * @param numberOfArticles The number of articles the producer needs to create.
 * @param queueSize The bound for it's queue size.
 */
void setValuesToProducers(int currentArrayMaxSize, int producerId, int numberOfArticles, int queueSize) {
    // Set the values to the current producer. Make it safe using the "if" statement.
    if (numProducers < currentArrayMaxSize) {
        producers[numProducers].producerId = producerId;
        producers[numProducers].numberOfArticles = numberOfArticles;
        producers[numProducers].queueSize = queueSize;
        // Increase the number of producers to the updated value.
        numProducers++;
    }
}

/**
 * A data allocation function using malloc.
 * @param amount The amount of objects to create.
 * @param sizeOfType The amount of bytes for each element.
 * @param pointerToAllocatedData The pointer to store the allocated data.
 */
void dataAllocation(int amount, int sizeOfType, void **pointerToAllocatedData) {
    // Allocate data.
    *pointerToAllocatedData = malloc(amount * sizeOfType);
    // Check if the malloc failed.
    if (*pointerToAllocatedData == NULL) {
        printf("Error in malloc\n");
        exit(ERROR);
    }
}

/**
 * Reading the configuration file into the global variables.
 * creating the producers array and setting the value of numProducers and coEditorQueueSize.
 * @param confPath The path to the configuration file.
 * @return
 */
int readConf(char *confPath) {
    // Open the configuration file.
    FILE *ConfFile = openFile(confPath);
    // Create an initiated value of the producers amount.
    int maxProducers = INIT_PROD_AMOUNT;
    // Initiate the producers to this amount.
    dataAllocation(maxProducers, sizeof(Producer), (void **) &producers);
//    initiateProducers(maxProducers);
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
        setValuesToProducers(maxProducers, producerId, numberOfArticles, queueSize);
        // Check for the need to resize the producers array.
        checkResizeProdArray(&maxProducers);
    }
    // If the loop ended, the producerId is actually the coEditorQueueSize.
    coEditorQueueSize = producerId;
    // Close the configuration file.
    fclose(ConfFile);

    // DELETE from here!!!!
    // Printing the read values for verification
    for (int i = 0; i < numProducers; i++) {
        printf("Producer %d:\n", producers[i].producerId);
        printf("Producer value: %d\n", producers[i].numberOfArticles);
        printf("Queue size: %d\n\n", producers[i].queueSize);
    }
    printf("Co-Editor queue size: %d\n", coEditorQueueSize);
    ValidateConfigurationFile();
    return 1;
}


/**
 * Generate all articles for a producer.
 * @param numberOfArticles The number of articles to produce.
 * @param producerId The producer id.
 * @param producerArticles The array to store the new articles.
 */
void generateArticle(int numberOfArticles, int producerId, Article **producerArticles) {
    // An array contains types of news.
    char *articlesType[NUM_OF_ARTICLES_TYPE] = {"SPORTS", "WEATHER", "NEWS"};
    // An array to store how many articles of this type where already generated.
    int articlesCount[NUM_OF_ARTICLES_TYPE] = {1, 1, 1};
    // Produce all articles.
    for (int i = 0; i < numberOfArticles; ++i) {
        // Generate random number modulo 3, what means it can be 0, 1 or 2.
        int randomNumber = rand() % 3;
        // Create a new pointer to Article and allocate space for it.
        Article *article;
        dataAllocation(1, sizeof(Article), (void *) &article);
        // Assign the producer id to the article.
        article->madeByProducerID = producers[producerId - CORRECTION].producerId;
        // Assign the type (string) of the article.
        strcpy(article->articleStr, articlesType[randomNumber]);
        // Assign the article's type (number) to the article.
        article->articleType = randomNumber;
        // Assign the number of articles produces so far from this type (not included).
        article->lastNumOfArticles = articlesCount[randomNumber];
        // Increase the number of articles produced for this type.
        articlesCount[randomNumber]++;
        // Assign the new article to the articles array.
        producerArticles[i] = article;
    }
}

Article* popFromBoundedQueue(BoundedQueue * boundedQueue) {
    sem_wait(&boundedQueue->full);
    pthread_mutex_lock(&boundedQueue->mutex);
    Article *article = boundedQueue->queueArticles[boundedQueue->consume];
    boundedQueue->consume = (boundedQueue->consume + 1) % boundedQueue->boundedQueueSize;
    pthread_mutex_unlock(&boundedQueue->mutex);
    sem_post(&boundedQueue->empty);
    return article;
}


void pushToBoundedQueue(Article *article, BoundedQueue *boundedQueue){
    sem_wait(&boundedQueue->empty);
    pthread_mutex_lock(&boundedQueue->mutex);
//    boundedQueue->queueArticles[boundedQueue->insert].madeByProducerID = article->madeByProducerID;
//    boundedQueue->queueArticles[boundedQueue->insert].lastNumOfArticles = article->lastNumOfArticles;
//    boundedQueue->queueArticles[boundedQueue->insert].articleType = article->articleType;
//    strcpy(boundedQueue->queueArticles[boundedQueue->insert].articleStr, article->articleStr);
    boundedQueue->queueArticles[boundedQueue->insert] = article;
    boundedQueue->insert = (boundedQueue->insert + 1) % boundedQueue->boundedQueueSize;
    pthread_mutex_unlock(&boundedQueue->mutex);
    sem_post(&boundedQueue->full);
    printf("id %d\n",article->madeByProducerID);
}


/**
 * Initiating the producers bounded queues.
 */
void initProducersQueues() {
    // Go over all producers.
    for (int i = 0; i < numProducers; i++) {
        // Allocate data for their articles queues.
        dataAllocation(producers[i].queueSize, sizeof(Article), (void **) &producers[i].boundedQueue.queueArticles);
        // Set the queue bound.
        producers[i].boundedQueue.boundedQueueSize = producers[i].queueSize;
        // Set the insert index to 0.
        producers[i].boundedQueue.insert = 0;
        // Set the "consume" index to 0.
        producers[i].boundedQueue.consume = 0;
        // Set the mutex to the bounded queue to 1.
        pthread_mutex_init(&producers[i].boundedQueue.mutex, NULL);
        // Set the semaphore empty to the queue size, to initiate it to all spots available.
        sem_init(&producers[i].boundedQueue.empty, 0, producers[i].queueSize);
        // Set the semaphore full to the queue size, to initiate it to all spots available.
        sem_init(&producers[i].boundedQueue.full, 0, 0);
    }
}


/**
 * Insert articles from the
 * @param producerId
 * @param producerArticles
 */
void insertArticles(int producerId, Article **producerArticles){
    for(int i = 0 ; i < producers[producerId - CORRECTION].numberOfArticles; i++){
        pushToBoundedQueue(producerArticles[i], &producers[producerId - CORRECTION].boundedQueue);
    }
}


void producerJob(void *arg) {
    int producerId = *(int *) arg;
    int numberOfArticles = producers[producerId - CORRECTION].numberOfArticles;
    Article **producerArticles;
    dataAllocation(numberOfArticles, sizeof(Article *), (void **) &producerArticles);
    generateArticle(numberOfArticles, producerId, producerArticles);
    insertArticles(producerId, producerArticles);
}


void createProducers() {
    pthread_t threads[numProducers];
    for (int i = 0; i < numProducers; i++) {
        int *id = malloc(sizeof(int));
        *id = i + CORRECTION;
        pthread_create(&threads[i], NULL, (void *(*)(void *)) producerJob, id);
    }

    // Wait for all threads to finish DELETE!
    for (int i = 0; i < numProducers; i++) {
        pthread_join(threads[i], NULL);
        printf("%d finished", i + 1);
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
    // Read the configuration file.
    readConf(argv[1]);
    // Initiate all producers bounded queues.
    initProducersQueues();
    // Seed the random number generator with the current time
    srand(time(NULL));
    // Create the producers threads and set them the data about the queues.
    createProducers();
    // Free the producers array.
    free(producers);
    return 0;
}
