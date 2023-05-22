#include "Producer.h"

/**
 * Creating new producer.
 * @param producerId The producer id.
 * @param numberOfArticles The number of articles it needs to create.
 * @param queueSize The bound for its queue size.
 * @return A pointer to the new producer on the heap.
 */
Producer *createProducer(int producerId, int numberOfArticles, int queueSize) {
    Producer *producer;
    dataAllocation(1, sizeof(Producer), (void*)&producer);//todo: Release!
    producer->producerId = producerId;
    producer->numberOfArticles = numberOfArticles;
    producer->queueSize = queueSize;
    return producer;
}

