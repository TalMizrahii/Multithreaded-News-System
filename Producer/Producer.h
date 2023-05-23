#ifndef OS_EX3_PRODUCER_H
#define OS_EX3_PRODUCER_H

#include "../ControlAndData/ControlAndData.h"
#include "../BoundedQueue/BoundedQueue.h"

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
