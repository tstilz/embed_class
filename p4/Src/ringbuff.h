
/** @file ringbuff.h
* 
* @brief Basic ring buffer.
*
* @par       
* COPYRIGHT NOTICE: (c) 2017 Barr Group.  
* All rights reserved.
*/ 

#ifndef __RINGBUFF_H
#define __RINGBUFF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>


#define DEF_BUFFSIZE  (128UL)
// *****************************************************************
typedef struct
{
    uint32_t  cookie;
    uint32_t  count;
    uint32_t  ritedex;
    uint32_t  readdex;
    uint32_t  capacity;
    char buffer[DEF_BUFFSIZE];
} RingBuffer;

// ****  Function prototypes for exported / public functions

extern void     RingBuffer_init(RingBuffer *me);
extern bool     RingBuffer_full(RingBuffer *me);
extern bool     RingBuffer_empty(RingBuffer *me);
extern uint32_t RingBuffer_capacity(RingBuffer *me);
extern uint32_t RingBuffer_space(RingBuffer *me);
extern char     RingBuffer_get(RingBuffer *me);
extern void     RingBuffer_put(RingBuffer *me, char val);



#ifdef __cplusplus
}
#endif

#endif /* __RINGBUFF_H */

