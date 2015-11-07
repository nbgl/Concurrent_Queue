//
//  Concurrent_Queue.h
//
//  Created by Jakub Nabaglo on 7/11/2015.
//  Released in public domain.
//

#ifndef Concurrent_Queue_h
#define Concurrent_Queue_h

#include "Bool.h"


#define CONCURRENT_QUEUE_SIZE \
(2*sizeof(void*)+sizeof(pthread_mutex_t)+sizeof(pthread_cond_t))
typedef struct {
    char __opaque[CONCURRENT_QUEUE_SIZE];
} ConcurrentQueue;

/* Initialises the queue. Must be called before any other function is called. */
void   concurrentQueueInit           (ConcurrentQueue *queue);

/* Destroys the queue. Must be called after all other functions have returned.
 Deallocating the queue without calling this function will result in a memory
 leak. */
void   concurrentQueueDestroy        (ConcurrentQueue *queue);

/* Pushes element to queue. Thread-safe. */
void   concurrentQueuePush           (ConcurrentQueue *queue, void *element);

/* Pops from the front of the queue. If the queue is empty, blocks until an
 element is added. Thread-safe. */
void  *concurrentQueuePop            (ConcurrentQueue *queue);

/* Peeks at front of the queue. If the queue is empty, blocks until an element
 is added. Thread-safe. */
void  *concurrentQueuePeek           (ConcurrentQueue *queue);

/* Attempts to pop from the front of the queue. If the queue is non-empty,
 returns the popped element and, if success is not NULL, sets *success to true.
 If the queue is empty, returns null and, if success is not NULL, sets *success
 to false. Thread-safe. */
void  *concurrentQueuePopNonblocking (ConcurrentQueue *queue, bool *success);

/* Attempts to peek at the front of the queue. If the queue is non-empty,
 returns the popped element and, if success is not NULL, sets *success to true.
 If the queue is empty, returns null and, if success is not NULL, sets *success
 to false. Thread-safe. */
void  *concurrentQueuePeekNonblocking(ConcurrentQueue *queue, bool *success);

#endif /* Concurrent_Queue_h */
