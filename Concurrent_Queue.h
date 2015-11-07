//
//  Concurrent_Queue.h
//
//  Created by Jakub Nabaglo on 7/11/2015.
//  Released in public domain.
//

#ifndef Concurrent_Queue_h
#define Concurrent_Queue_h

#include "Bool.h"
#include "Concurrent_Queue_Element_Type.h"


#define CONCURRENT_QUEUE_SIZE \
(2*sizeof(void*)+sizeof(pthread_mutex_t)+sizeof(pthread_cond_t))
typedef struct {
    char __opaque[CONCURRENT_QUEUE_SIZE];
} ConcurrentQueue;

/* Initialises the queue. Must be called before any other function is called. */
void concurrentQueueInit(ConcurrentQueue *queue);

/* Destroys the queue. Must be called after all other functions have returned.
 Deallocating the queue without calling this function will result in a memory
 leak. */
void concurrentQueueDestroy(ConcurrentQueue *queue);

/* Pushes element to queue. Thread-safe. */
void concurrentQueuePush(ConcurrentQueue *queue,
                         CONCURRENT_QUEUE_TYPE element);

/* Pops from the front of the queue. If the queue is empty, blocks until an
 element is added. Thread-safe. */
CONCURRENT_QUEUE_TYPE concurrentQueuePop(ConcurrentQueue *queue);

/* Peeks at front of the queue. If the queue is empty, blocks until an element
 is added. Thread-safe. */
CONCURRENT_QUEUE_TYPE concurrentQueuePeek(ConcurrentQueue *queue);

/* Attempts to pop from the front of the queue. If the queue is non-empty,
 returns the popped element and, if success is not NULL, sets *success to true.
 If the queue is empty, returns CONCURRENT_QUEUE_EMPTY_DEFAULT_VALUE and, if
 success is not NULL, sets *success to false. Thread-safe. */
CONCURRENT_QUEUE_TYPE concurrentQueuePopNonblocking (ConcurrentQueue *queue,
                                                     bool *success);

/* Attempts to peek at the front of the queue. If the queue is non-empty,
 returns the popped element and, if success is not NULL, sets *success to true.
 If the queue is empty, returns CONCURRENT_QUEUE_EMPTY_DEFAULT_VALUE and, if
 success is not NULL, sets *success to false. Thread-safe. */
CONCURRENT_QUEUE_TYPE concurrentQueuePeekNonblocking(ConcurrentQueue *queue,
                                                     bool *success);

#endif /* Concurrent_Queue_h */
