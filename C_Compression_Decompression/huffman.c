/*
 * huffman.c
 * CMPS 12B: Sprint 2017
 */

# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>
# include <ctype.h>
# include <errno.h>
# include <string.h>
# include <unistd.h>

# include "huffman.h"
# include "stack.h"

# define DOLLARSIGN 36
# define SIZEOFBYTE 8

treeNode ** reference;  // global pointer to members of Huffman tree

// New node, with symbols, leaf or not, a count associated with it
// Written by Cori Calabi
treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
    treeNode *thisNode;
    thisNode = (treeNode *) calloc(1, sizeof(treeNode));
    if (thisNode)
    {
        thisNode->symbol = s;
        thisNode->count = c;
        thisNode->leaf = l;
        thisNode->left = NIL;
        thisNode->right = NIL;
        return thisNode;
    }
    else 
    {
        printf("Error in calloc: thisNode [%d: %s]\n", errno, strerror(errno));
        return (treeNode *) NIL;
    }
}

// Dump a Huffman tree onto a file
// Written by Cori Calabi
void dumpTree(treeNode *t, int file)
{
    if (t->leaf != true)
    {
        dumpTree(t->left, file);
        dumpTree(t->right, file);
    }
    int bytesWritten = 0;
    if (t && t->leaf)
    {
        uint8_t thisChar = t->symbol;
        uint8_t leafN = 'L';
        bytesWritten = write(file, &leafN, 1);
        if (bytesWritten == -1)
        {
            printf("Could not input bitVector\n");
        //    perror(argv[0]);
            exit(errno);
        }
        bytesWritten =  write(file, &thisChar, 1);
        if (bytesWritten == -1)
        {
            printf("Could not input bitVector\n");
        //    perror(argv[0]);
            exit(errno);
        }
    }
    else if (t)
    {
        uint8_t internalN = 'I';
        bytesWritten = write(file, &internalN, 1);
        if (bytesWritten == -1)
        {
            printf("Could not input bitVector\n");
          //  perror(argv[0]);
            exit(errno);
        }
    }
    return;
}

// Build a tree from the saved tree
// Author: Ryan Hamamoto
treeNode *loadTree (uint8_t savedTree [], uint16_t treeBytes)
{
	stack *stack = newStack(); //create a new stack
	for (uint8_t i = 0; i < treeBytes; i += 1)
	{
		if(savedTree[i] == 'L')
		{
			i += 1;
		        treeNode *leaf = newNode(savedTree[i], true, 0); //create a new leaf node
			push(stack, leaf); //push this node onto the stack
		}
		else if(savedTree[i] == 'I')
		{
			treeNode *right = pop(stack); //pop 2 children off the stack
			treeNode *left = pop(stack);
			treeNode *interior = join(left, right); //make them children of the interior node
			push(stack, interior); //push interior node back onto the stack
		}
		else
		{
				printf("\nERROR: Cannot create tree\n");
				exit(errno);
		}
		
	}
	treeNode *root = pop(stack);
	delStack(stack);
	return root; //the last item on the stack is now the root
}

//helper function to attain the next bit in the 8 bit byte
//Author: Ryan Hamamoto
static inline bool getNextBit(uint8_t byteRead, uint8_t position)
{
	bool ret;
	if((byteRead & (0x1 << position))  != 0)
	{
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	return ret;
}

// Step through a tree following the code
//Author: Ryan Hamamoto
int32_t stepTree (treeNode *root, uint8_t code, char outFile [])
{	
	extern treeNode **reference;
	int32_t leafFlag = -1; //unless reset, indicates no leaf encountered
	static uint64_t outFileIndex = 0;
	for(uint8_t i = 0; i < SIZEOFBYTE; i += 1) //count through each bit of byte
	{
		bool output = getNextBit(code, i);
		if(output == 1)
		{
			reference = &((*reference)->right);
		}
		else if(output == 0)
		{
			reference = &((*reference)->left);
		}
		if((*reference)->leaf == true) //if the step function encountered a leaf
		{
			outFile[outFileIndex] = (*reference)->symbol; //save the symbol
			reference = &root; //reset index to the root
			outFileIndex += 1; //increment index of the output array
			leafFlag = 1; //flag that a leaf was encountered
		}
	}
	return leafFlag;
	
}

// Parse a Huffman tree to build codes
// Author: Cori Calabi
void buildCode(treeNode *n, code s, code table[256])
{
    if (n != NIL)
    {
        table[n->symbol] = s;
    }
   // printf("buildCode\n");
    return;
}

// Delete a tree
// Author: Ryan Hamamoto
void delTree(treeNode *t)
{
	//step through tree and delete each node
	if (t != NIL) //as long as t is a valid pointer
	{
		delTree(t->left); //delete left tree (left child treated as root)
		delTree(t->right); //delete right tree (right child treated as root)
		delNode(t); //only delete nodes with no children
	}
	return; //return back to previous delTree recursions once t == NIL
}

//creates an internal node with l and r as its two children
//Author: Ryan Hamamoto
treeNode *join(treeNode *l, treeNode *r) // Join two subtrees and create an interior node
{
	treeNode *root = newNode(DOLLARSIGN, false, l->count + r->count); //create new root
	int8_t ratio = compare(l, r); //determine the ratio of left count and right count
	if(ratio > 0)
	{
		//l > r
		root->left = r;
		root->right = l;
	}
	else if(ratio <= 0) //default when decoding and creating tree without counts
	{
		//r > l or r = l
		root->left = l;
		root->right = r;
	}
	return root;
}

//Author of printTree and spaces: Darrell Long
static inline void spaces(int c) { for (int i = 0; i < c; i += 1) { putchar(' '); } return; }

void printTree(treeNode *t, int depth)
{
    if (t)
    {
        printTree(t->left, depth + 1);
        if (t->leaf)
        {
            if (isalnum(t->symbol))
            {
                spaces(4 * depth);
                printf("'%c' (%lu)\n", t->symbol, t->count);
            }
            else
            {
                spaces(4 * depth);
                printf("0x%X (%lu)\n", t->symbol, t->count);
            }
        }
        else
        {
            spaces(4 * depth);
            printf("$ (%lu)\n", t->count);
        }
        printTree(t->right, depth + 1);
    }
    return;
}
