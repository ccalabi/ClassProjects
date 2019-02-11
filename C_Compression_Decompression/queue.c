// queue.c: Written by Darrell Long (from lecture 5/18/17 and Piazza post 850)
// With changes/additions by Cori Calabi
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>

# include "queue.h"
# include "huffman.h"

queue *newQueue(uint32_t size)
{
    queue *thisQ;
    thisQ = (queue *) calloc(1, sizeof(queue));
    thisQ->size = size;
    thisQ->head = 0;
    thisQ->tail = 0;
    thisQ->Q = (treeNode **)calloc(size, sizeof(treeNode *));
    if (thisQ)
    {
        return thisQ;
    }
    else
    {
        printf("newQueue failed.\n");
        return (queue *)NULL;
    }
}

void delQueue(queue *q)
{
    free(q->Q);
    free(q);
    return;
}

bool empty(queue *q)
{
    return (q->head == q->tail);
}

bool full(queue *q)
{
    return (q->tail + 1) % q->size == q->head % q->size;
}

// Add an item to the end of the queue
bool enqueue(queue *q, treeNode *i)
{
    if (full(q))
    {
        printf("Queue full!\n");
        return false;
    }
    else
    {
        q->Q[q->tail] = i;
        i = NULL;
        q->tail = (q->tail + 1) % q->size;
        return true;
    }
}

// Remove an item from the front of the queue
treeNode *dequeue(queue *q, treeNode *i)
{
    if (empty(q))
    {
        return (treeNode *)NULL;
    }
    else
    {
        uint32_t tHead = q->head;
        //printf("Head: %u, tail: %u\n", q->head, q->tail);
        i = q->Q[tHead];
        q->Q[tHead] = (treeNode *)NULL;
        q->head = (tHead + 1) % q->size;
        return i;
    }
}
