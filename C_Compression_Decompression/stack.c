/* stack.c
 * Author: Cori Calabi & Ryan Hamamoto, based on Darrell Long's code in Piazza post 219 & 851
 */

# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>

# include "stack.h"
# include "huffman.h"

# define MINSTACK 10

stack *newStack(void)
{
    stack *thisStack = (stack *)calloc(MINSTACK, sizeof(stack));
    thisStack->size = MINSTACK;
    thisStack->top = 0;
    thisStack->entries = (treeNode **)calloc(MINSTACK, sizeof(treeNode *));
    if (thisStack)
    {
        return thisStack;
    }
    else
    {
        printf("\nnewStack failed.\n");
        return (stack *)NIL;
    }
}

void delStack(stack *s)
{
    free(s->entries);
    free(s);
    return;
}

treeNode *pop(stack *s)
{
    if (s->top > 0)
    {
        s->top -= 1;
        return s->entries[s->top];
    }
    else
    {
	printf("\npop failed.\n");         
	return (treeNode *)NIL;
    }
}

void push(stack *s, treeNode *entry)
{
    if (s->top == s->size)
    {
        s->size *= 2;
        s->entries = (treeNode **) realloc(s->entries, s->size * sizeof(treeNode *));
    }
    s->entries[s->top] = entry;
    s->top += 1;
    return;
}

bool emptyStack(stack *s)
{
    return s->top == 0;
}

bool fullStack(stack *s)
{
    return s->top == s->size;
}
