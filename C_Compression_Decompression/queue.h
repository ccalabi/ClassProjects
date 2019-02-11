// queue.h: Written by Darrell Long

# ifndef _QUEUE_H
# define _QUEUE_H

# include <stdint.h>
# include <stdbool.h>

# include "huffman.h"

typedef struct queue
{
    uint32_t size;  // How big is it?
    uint32_t head, tail;  // Front and rear locations
    treeNode **Q;  // Array to hold queue (via calloc)
    //Arjun recommended making this an array of pointers 
    //instead of an array of "items"
} queue;

queue *newQueue(uint32_t size);  // Constructor
void delQueue(queue *q);  // Destructor

bool empty(queue *q);  // Is it empty?
bool full(queue *q);  // Is it full?

bool enqueue(queue *q, treeNode *i);  // Add an item
treeNode *dequeue(queue *q, treeNode *i);  // Remove from rear

# endif
