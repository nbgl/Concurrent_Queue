//
//  Concurrent_Queue.c
//
//  Created by Jakub Nabaglo on 7/11/2015.
//  Released in public domain.
//

#include <pthread.h>
#include <stdlib.h>
#include "Bool.h"

typedef struct _LinkedListElement LinkedListElement;
struct _LinkedListElement {
    void *element;
    LinkedListElement *next;
};

typedef struct {
    LinkedListElement  *start;
    LinkedListElement **endPointer;
    pthread_mutex_t mutex;
    pthread_cond_t nonemptyCond;
} ConcurrentQueue;

void concurrentQueueInit(ConcurrentQueue *queue) {
    queue->start = NULL;
    queue->endPointer = &queue->start;
    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->nonemptyCond, NULL);
}
void concurrentQueueDestroy(ConcurrentQueue *queue) {
    LinkedListElement *start = queue->start;
    while (start) { // deallocate all the elements
        LinkedListElement *next = start->next;
        free(start);
        start = next;
    }
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->nonemptyCond);
}

void concurrentQueuePush(ConcurrentQueue *queue, void *element) {
    pthread_mutex_lock(&queue->mutex);
    
    LinkedListElement *newEnd = malloc(sizeof(LinkedListElement));
    *queue->endPointer = newEnd;
    queue->endPointer = &newEnd->next;
    
    newEnd->element = element;
    newEnd->next = NULL;
    
    pthread_cond_signal(&queue->nonemptyCond);
    pthread_mutex_unlock(&queue->mutex);
}

void *concurrentQueuePeek(ConcurrentQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    void *result;
    if (!queue->start) {
        pthread_cond_wait(&queue->nonemptyCond, &queue->mutex);
    }
    result = queue->start->element;
    pthread_mutex_unlock(&queue->mutex);
    return result;
}

void *concurrentQueuePop(ConcurrentQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    void *result;
    if (!queue->start) {
        pthread_cond_wait(&queue->nonemptyCond, &queue->mutex);
    }
    result = queue->start->element;
    LinkedListElement *oldStart = queue->start;
    if (queue->endPointer == &oldStart->next) {
        queue->start = NULL;
        queue->endPointer = &queue->start;
    } else {
        queue->start = oldStart->next;
    }
    free(oldStart);
    pthread_mutex_unlock(&queue->mutex);
    return result;
}

void *concurrentQueuePopNonblocking(ConcurrentQueue *queue, bool *success) {
    pthread_mutex_lock(&queue->mutex);
    void *result;
    if (!queue->start) {
        if (success) {
            (*success) = false;
        }
        result = NULL;
    } else {
        result = queue->start->element;
        LinkedListElement *oldStart = queue->start;
        if (queue->endPointer == &oldStart->next) {
            queue->start = NULL;
            queue->endPointer = &queue->start;
        } else {
            queue->start = oldStart->next;
        }
        free(oldStart);
    }
    pthread_mutex_unlock(&queue->mutex);
    return result;
}

void *concurrentQueuePeekNonblocking(ConcurrentQueue *queue, bool *success) {
    pthread_mutex_lock(&queue->mutex);
    void *result;
    if (!queue->start) {
        if (success) {
            (*success) = false;
        }
        result = NULL;
    } else {
        result = queue->start->element;
    }
    pthread_mutex_unlock(&queue->mutex);
    return result;
}