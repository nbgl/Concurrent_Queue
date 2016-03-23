//
//  Concurrent_Queue.c
//
//  Created by Jakub Nabaglo on 7/11/2015.
//  Released in public domain.
//

#ifndef Concurrent_Queue_Element_Type_h
#define Concurrent_Queue_Element_Type_h

/* The type of the elements of the queue. Change to whatever you need. */
#define CONCURRENT_QUEUE_TYPE void*

/* The value returned by the nonblocking functions if the queue is empty. Must
 be of the CONCURRENT_QUEUE_TYPE type. Change to whatever you need. */
#define CONCURRENT_QUEUE_EMPTY_DEFAULT_VALUE NULL

/* Examples:
 1. #define CONCURRENT_QUEUE_TYPE                void*
    #define CONCURRENT_QUEUE_EMPTY_DEFAULT_VALUE NULL
 
 2. #define CONCURRENT_QUEUE_TYPE                uint32_t
    #define CONCURRENT_QUEUE_EMPTY_DEFAULT_VALUE UINT32_MAX
 
 3. #define CONCURRENT_QUEUE_TYPE                char
    #define CONCURRENT_QUEUE_EMPTY_DEFAULT_VALUE 0
*/

#endif /* Concurrent_Queue_Element_Type_h */
