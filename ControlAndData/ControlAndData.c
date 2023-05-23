#include "ControlAndData.h"

/**
 * A destructor for the article.
 * @param article A pointer to an article.
 */
void destroyArticle(Article *article) {
    // Deallocate the article.
    free(article);
    // Set its pointer to null.
    article = NULL;
}

/**
 * Create an article using the producerId, articlesType for its title,
 * and the articlesCount to track how many was made from this type.
 * @param producerId The ID of the producer who made the article.
 * @param articlesType The types of what the article can be (string).
 * @param articlesCount The count for the article (how many made so far).
 * @return A pointer to the article on the heap.
 */
Article *createArticle(int producerId, char *articlesType, int articleCount, int serial) {
    // Create a new article and allocate data for it.
    Article *article;
    dataAllocation(1, sizeof(Article), (void *) &article);
    // Assign the producer id to the article.
    article->madeByProducerID = producerId;
    // Assign the type (string) of the article.
    strcpy(article->articleStr, articlesType);
    // Assign the number of articles produces so far from this type (not included).
    article->lastNumOfArticles = articleCount;
    // Set the serial number of the article.
    article->serial = serial;
    return article;
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