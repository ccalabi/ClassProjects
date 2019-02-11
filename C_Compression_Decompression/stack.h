# ifndef _STACK_H
# define _STACK_H
# include <stdint.h>
# include <stdbool.h>
# ifndef NIL
# define NIL (void *)0
# endif

# include "huffman.h"

typedef struct stack
{
	uint32_t size; // How big ?
	uint32_t top; // Where ’s the top ?
	treeNode **entries; // Array to hold it ( via calloc )
} stack;

stack *newStack(void);

void delStack (stack *s); // Destructor

treeNode *pop(stack *s);
// Returns the top item
void push (stack *s, treeNode *entry) ; // Adds an item to the top

bool emptyStack (stack *s); //is it empty?
bool fullStack (stack *s); //is it full?

# endif
