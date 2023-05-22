#ifndef OS_EX3_PRODUCER_H
#define OS_EX3_PRODUCER_H

#include "ControlAndData.h"

// Define the producer as an id, the number of articles he has to produce, and the size of his bounded queue size.
typedef struct {
    int producerId;
    int numberOfArticles;
    int queueSize;
} Producer;

Producer *createProducer(int producerId, int numberOfArticles, int queueSize);

#endif //OS_EX3_PRODUCER_H