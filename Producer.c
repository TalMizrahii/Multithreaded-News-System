#include "Producer.h"
//
//
///**
// * Generate all articles for a producer.
// * @param numberOfArticles The number of articles to produce.
// * @param producerId The producer id.
// * @param producerArticles The array to store the new articles.
// */
//void generateArticle(int numberOfArticles, int producerId, Article **producerArticles) {
//    // An array contains types of news.
//    char *articlesType[NUM_OF_ARTICLES_TYPE] = {"SPORTS", "WEATHER", "NEWS"};
//    // An array to store how many articles of this type where already generated.
//    int articlesCount[NUM_OF_ARTICLES_TYPE] = {1, 1, 1};
//    // Produce all articles.
//    for (int i = 0; i < numberOfArticles; ++i) {
//        // Generate random number modulo 3, what means it can be 0, 1 or 2.
//        int randomNumber = rand() % 3;
//        // Create a new pointer to Article and allocate space for it.
//        Article *article;
//        dataAllocation(1, sizeof(Article), (void *) &article);
//        // Assign the producer id to the article.
//        article->madeByProducerID = producers[producerId - CORRECTION].producerId;
//        // Assign the type (string) of the article.
//        strcpy(article->articleStr, articlesType[randomNumber]);
//        // Assign the article's type (number) to the article.
//        article->articleType = randomNumber;
//        // Assign the number of articles produces so far from this type (not included).
//        article->lastNumOfArticles = articlesCount[randomNumber];
//        // Increase the number of articles produced for this type.
//        articlesCount[randomNumber]++;
//        // Assign the new article to the articles array.
//        producerArticles[i] = article;
//    }
//}



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

