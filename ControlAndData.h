
#ifndef OS_EX3_CONTROLANDDATA_H
#define OS_EX3_CONTROLANDDATA_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ERROR (-1)
#define MAX_SIZE_OF_NEWS_STR 20
#define NEXT_LOCATION 1
#define ARGUMENTS_NUM 2
#define ERROR (-1)
#define DOUBLE 2
#define TRUE 1
#define NUM_OF_ARTICLES_TYPE 3

typedef struct {
    char articleStr[MAX_SIZE_OF_NEWS_STR];
    int madeByProducerID;
    int lastNumOfArticles;
    int serial;
} Article;

void dataAllocation(int amount, int sizeOfType, void **pointerToAllocatedData);

#endif //OS_EX3_CONTROLANDDATA_H
