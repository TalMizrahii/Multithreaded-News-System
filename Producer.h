#ifndef OS_EX3_PRODUCER_H
#define OS_EX3_PRODUCER_H

#include "ControlAndData.h"
#include "BoundedQueue.h"

// Define the producer as an id, the number of articles he has to produce, and the size of his bounded queue size.
typedef struct {
    int producerId;
    int numberOfArticles;
    int queueSize;
} Producer;

typedef struct {
    Producer *producer;
    BoundedQueue *boundedQueue;
} ProducerJobArgs;

Producer *createProducer(int producerId, int numberOfArticles, int queueSize);

void destroyProducer(Producer *producer);

void destroyProducerJobArgs(ProducerJobArgs *producerJobArgs);

void *producerJob(void *producerJobArgs);

#endif //OS_EX3_PRODUCER_H