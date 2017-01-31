

/** \file ringbuff.c
*
* @brief /Rudimentary/ ring buffer.
*        Everyone has his opinion; write your own if you don't like this one.
*
* @par
* COPYRIGHT NOTICE: (c) 2017 Barr Group, LLC.
* All rights reserved.
*/


#include <stdint.h>    // uint32_t etc.
#include <stdbool.h>   // bool
#include "project.h"
#include "ringbuff.h"

#define INIT_COOKIE   (0x0BADC0DE)
#define INIT_OK(me)   (INIT_COOKIE == me->cookie)

// *****************************************************************
void RingBuffer_init(RingBuffer *me)
{
  // sanity check
  my_assert(me);
  me->cookie    = INIT_COOKIE;
  me->count     = 0UL;
  me->ritedex   = 0UL;
  me->readdex   = 0UL;
  me->capacity  = (sizeof(me->buffer) / (sizeof(me->buffer[0])));
}

// *****************************************************************
bool RingBuffer_empty(RingBuffer *me)
{
  my_assert(INIT_OK(me)); // sanity check
  return (0 == me->count);
}

// *****************************************************************
bool RingBuffer_full(RingBuffer *me)
{
  my_assert(INIT_OK(me)); // sanity check
  return (me->count == me->capacity);
}

// *****************************************************************
void RingBuffer_put(RingBuffer *me, char val)
{
  my_assert(INIT_OK(me)); // sanity checks
  my_assert(me->ritedex < me->capacity);
  my_assert(me->count <= me->capacity);

  // If full, silently discard data
  if (!RingBuffer_full(me))
  {
    me->count++;
    me->buffer[me->ritedex++] = val;
    my_assert(me->ritedex <= me->capacity);
    if (me->ritedex == me->capacity)  // index wrap
    {
      me->ritedex = 0;
    }
  }
}

// *****************************************************************
char RingBuffer_get(RingBuffer *me)
{
  my_assert(INIT_OK(me)); // sanity checks
  my_assert(me->readdex < me->capacity);
  my_assert(me->count <= me->capacity);

  if (RingBuffer_empty(me))
  {
    return (char)0;
  }

  my_assert(me->count);
  me->count--;
  char val = me->buffer[me->readdex++];
  my_assert(me->readdex <= me->capacity);
  if (me->readdex == me->capacity)
  {
    me->readdex = 0;
  }
  return val;
}

// *****************************************************************
uint32_t RingBuffer_capacity(RingBuffer *me)
{
  my_assert(INIT_OK(me)); // sanity check
  return me->capacity;
}

// *****************************************************************
uint32_t RingBuffer_space(RingBuffer *me)
{
  my_assert(INIT_OK(me)); // sanity checks
  my_assert(me->capacity >= me->count);
  return (me->capacity - me->count);
}

