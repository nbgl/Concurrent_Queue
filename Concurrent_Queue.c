//
//  Concurrent_Queue.c
//
//  Created by Jakub Nabaglo on 7/11/2015.
//  Released in public domain.
//

#include <pthread.h>
#include <stdlib.h>
#include "Bool.h"
#include "Concurrent_Queue_Element_Type.h"


typedef struct _LinkedListElement LinkedListElement;
struct _LinkedListElement {
    CONCURRENT_QUEUE_TYPE element;
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
    while (start) { /* deallocate all the elements */
        LinkedListElement *next = start->next;
        free(start);
        start = next;
    }
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->nonemptyCond);
}

void concurrentQueuePush(ConcurrentQueue *queue, CONCURRENT_QUEUE_TYPE element) {
    pthread_mutex_lock(&queue->mutex);
    
    LinkedListElement *newEnd = malloc(sizeof(LinkedListElement));
    *queue->endPointer = newEnd;
    queue->endPointer = &newEnd->next;
    
    newEnd->element = element;
    newEnd->next = NULL;
    
    pthread_cond_signal(&queue->nonemptyCond);
    pthread_mutex_unlock(&queue->mutex);
}

CONCURRENT_QUEUE_TYPE concurrentQueuePeek(ConcurrentQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    if (!queue->start) /* queue empty */ {
        pthread_cond_wait(&queue->nonemptyCond, &queue->mutex);
    }
    
    CONCURRENT_QUEUE_TYPE result = queue->start->element;
    
    pthread_mutex_unlock(&queue->mutex);
    return result;
}

CONCURRENT_QUEUE_TYPE concurrentQueuePop(ConcurrentQueue *queue) {
    pthread_mutex_lock(&queue->mutex);
    if (!queue->start) /* queue empty */ {
        pthread_cond_wait(&queue->nonemptyCond, &queue->mutex);
    }
    
    LinkedListElement *oldStart = queue->start;
    CONCURRENT_QUEUE_TYPE result = oldStart->element;
    if (queue->endPointer == &oldStart->next) {
        /* popping the only element */
        queue->start = NULL;
        queue->endPointer = &queue->start;
    } else {
        /* still elements left */
        queue->start = oldStart->next;
    }
    free(oldStart);
    
    pthread_mutex_unlock(&queue->mutex);
    return result;
}

CONCURRENT_QUEUE_TYPE concurrentQueuePopNonblocking(ConcurrentQueue *queue,
                                                    bool *success) {
    pthread_mutex_lock(&queue->mutex);
    
    bool empty = !queue->start;
    if (success) {
        *success = !empty;
    }
    
    CONCURRENT_QUEUE_TYPE result;
    if (empty) /* queue empty */ {
        result = CONCURRENT_QUEUE_EMPTY_DEFAULT_VALUE;
    } else /* queue nonempty */ {
        LinkedListElement *oldStart = queue->start;
        result = oldStart->element;
        if (queue->endPointer == &oldStart->next) {
            /* popping the only element */
            queue->start = NULL;
            queue->endPointer = &queue->start;
        } else {
            /* still elements left */
            queue->start = oldStart->next;
        }
        free(oldStart);
    }
    
    pthread_mutex_unlock(&queue->mutex);
    return result;
}

CONCURRENT_QUEUE_TYPE concurrentQueuePeekNonblocking(ConcurrentQueue *queue,
                                                     bool *success) {
    pthread_mutex_lock(&queue->mutex);
    
    bool empty = !queue->start;
    if (success) {
        *success = !empty;
    }
    
    CONCURRENT_QUEUE_TYPE result = empty
        ? CONCURRENT_QUEUE_EMPTY_DEFAULT_VALUE
        : queue->start->element;
    
    pthread_mutex_unlock(&queue->mutex);
    return result;
}