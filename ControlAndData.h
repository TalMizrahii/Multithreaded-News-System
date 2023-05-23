#ifndef OS_EX3_CONTROLANDDATA_H
#define OS_EX3_CONTROLANDDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


#define ERROR (-1)
#define MAX_SIZE_OF_NEWS_STR 20
#define NEXT_LOCATION 1
#define ARGUMENTS_NUM 2
#define ERROR (-1)
#define DOUBLE 2
#define TRUE 1
#define NUM_OF_ARTICLES_TYPE 3
#define TENTH_SEC 100000
#define DONE (-1)
#define SPORTS 0
#define WEATHER 1
#define NEWS 2

typedef struct {
    char articleStr[MAX_SIZE_OF_NEWS_STR];
    int madeByProducerID;
    int lastNumOfArticles;
    int serial;
} Article;

void dataAllocation(int amount, int sizeOfType, void **pointerToAllocatedData);

void ValidateConfigurationFile(int numProducers, int coEditorQueueSize);

FILE *openFile(char *filePath);

void argCheck(int argc);

#endif //OS_EX3_CONTROLANDDATA_H
