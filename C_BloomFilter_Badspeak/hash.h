//hash.h: Author: Darrell Long

# ifndef NIL
# define NIL (void *) 0
# endif

# ifndef _HASH_H
# define _HASH_H

# include <stdint.h>
# include "ll.h"

extern int callLL;

typedef struct hashTable hashTable;

struct hashTable
{
    uint32_t s[4];  //Salt
    uint32_t l;     //Length
    listNode **h;   //Array of pointers to linked lists
};

hashTable *newHT(uint32_t, uint32_t []);

void delHT(hashTable *);

listNode *findHT(hashTable *, const char *);

void insertHT(hashTable *, const char *, const char *);

uint32_t hash(hashTable *, const char *);


void printHT(const hashTable *);

# endif
