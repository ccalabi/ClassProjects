// hash.c: Make and manipulate hash tables
// Written by Cori Calabi, based on hash.h by Darrell Long
# include <stdlib.h>
# include <stdint.h>
# include <string.h>
# include <stdio.h>

# include "aes.h"
# include "hash.h"
# include "ll.h"


// Written by Darrell Long: Find the actual length of a word
static inline int realLength(int l)
{
    return 16 * (l / 16 + (l % 16 ? 1 : 0));
}

// Delete a node of a hash table
hashTable *newHT(uint32_t length, uint32_t salt[])
{
    struct hashTable *hashT;
    hashT = (hashTable *) malloc(sizeof(hashTable));
    hashT->h = (listNode **) calloc(length, sizeof(listNode *));
    for (int i = 0; i < 4; i++)
    {
        hashT->s[i] = salt[i];
    }
    hashT->l = length;
    return hashT;
}

// Delete a hash table
void delHT(hashTable *hashT)
{
    for (uint32_t i = 0; i < hashT->l; i++)
    {
        delLL(hashT->h[i]);
    }
    free(hashT->h);
    free(hashT);
    return;
}

// Find an the listNode of an oldspeak word in the hash table
listNode *findHT(hashTable *hashT, const char *oldW)
{
    uint32_t hashed = 0;
    hashed = hash(hashT, oldW);
    hashed = hashed % hashT->l;
//    listNode **dPtr = NIL; 
//    listNode *ptr = NIL;
//    ptr = h[hashed];
//    dPtr = &ptr;
      return findLL(&hashT->h[hashed], oldW);
}

// Add an oldspeak word to the hash table at the correct index
void insertHT(hashTable *hashT, const char *oldW, const char *newW)
{
    uint32_t hashed = 0;
    hashed = hash(hashT, oldW);
    hashed = hashed % hashT->l;
//    listNode **ptr = NIL;
//    ptr = &h[hashed];
//    insertLL(&findLL(&hashT->h[hashed], oldW), oldW, newW);
    insertLL(&hashT->h[hashed], oldW, newW);
    return;
}

// hash() written by Darrell Long
// Hash the oldspeak word to get a number
uint32_t hash(hashTable *h, const char *key)
{
    uint32_t output[4] = { 0x0 };
    uint32_t sum = 0x0;
    int keyL = strlen(key);
    uint8_t *realKey = (uint8_t *) calloc(realLength(keyL), sizeof(uint8_t));

    memcpy(realKey, key, keyL);

    for (int i = 0; i < realLength(keyL); i += 16)
    {
        AES128_ECB_encrypt((uint8_t *) h->s,  //Salt
                           (uint8_t *) realKey + i,  //Input
                           (uint8_t *) output);  //Output
        sum ^= output[0] ^ output[1] ^ output[2] ^ output[3];
    }
    free(realKey);
    return sum;
}

//Print the hashtable
void printHT(const hashTable *h)
{
    for (uint32_t i = 0; i < h->l; i++)
    {
        if (h->h[i] != NIL)
        {
            printf("h[%u]: ", i);
            printLL(h->h[i]);
        }
    }
    return;
}
