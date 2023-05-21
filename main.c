#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARGUMENTS_NUM 2
#define ERROR (-1)
#define INIT_PROD_AMOUNT 1
#define DOUBLE 2

// Define the producer as an id, the number of articles he has to produce, and the size of his bounded queue size.
typedef struct {
    int producerId;
    int numberOfArticles;
    int queueSize;
} Producer;

typedef struct {
    int madeByProducerID;
    char *articleStr;
    int articleType;
} Article;


// The array of producers.
Producer *producers;
// The array of queues contains the articles from the producers.
Article ***rawArticles;
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
 * Initiating the producers array.
 * @param maxProducers The maximum value of the array to initiate.
 */
void initiateProducers(int maxProducers) {
    // Initiate the array.
    producers = malloc(maxProducers * sizeof(Producer));
    // Check if the malloc failed.
    if (producers == NULL) {
        printf("Error in malloc\n");
        exit(ERROR);
    }
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
 * Initiating the producers queues.
 */
void initProducersQueues() {
    for (int i = 0; i < numProducers; i++) {
        dataAllocation(producers[i].queueSize, sizeof(Article *), (void **) &rawArticles[i]);
    }
}

void producerJob(int id) {
    int randomNumber = rand() % 3;

}


void createProducers() {
    for (int i = 0; i < numProducers; i++) {
        producerJob(i + 1);
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
    // Initiate the common array of data between the dispatcher and the producers.
    dataAllocation(numProducers, sizeof(Article *), (void *) &rawArticles); // Need To FREE!!!
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
