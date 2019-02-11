//bf.h: Written by Cori Calabi (based on outline & function names by Darrell Long)
//Bloom filter

# ifndef NIL
# define NIL (void *) 0
# endif

# ifndef _BF_H
# define _BF_H

# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# include "hash.h"
# include "aes.h"
# include "ll.h"

typedef struct bloomF 
{
    uint8_t *v; //Vector
    uint32_t l; //Length
    uint32_t s[4]; //Salt
} bloomF;

//Salts (by Darrell Long)
uint32_t initA[] = {0xDeadD00d, 0xFadedBee,
                    0xBadAb0de, 0xC0c0aB0a};  //First Bloom filter
uint32_t initB[] = {0xDeadBeef, 0xFadedB0a,
                    0xCafeD00d, 0xC0c0aB0a};  //Second Bloom filter
uint32_t initH[] = {0xDeadD00d, 0xFadedBee,
                    0xBadAb0de, 0xC0c0Babe};  //Hash table

//Each function has its own hash function, determined by the salt.
/* hashBF() takes in a bloom filter and a word (pointer to) and returns 
 * the hashed value of the word
 * This function based on hash() in hash.c, written by Darrell Long
 */
uint32_t hashBF(bloomF *x, char *key)
{
    uint32_t output[4] = { 0x0 };
    uint32_t sum = 0x0;
    int keyL = strlen(key);
    int realLength = 16 * (keyL / 16 + (keyL % 16 ? 1 : 0));
    uint8_t *realKey = (uint8_t *) calloc(realLength, sizeof(uint8_t));

    memcpy(realKey, key, keyL);

    for (int i = 0; i < realLength; i += 16)
    {
        AES128_ECB_encrypt((uint8_t *) x->s,  //Salt
                           (uint8_t *) realKey + i,  //Input
                           (uint8_t *) output);  //Output
        sum ^= output[0] ^ output[1] ^ output[2] ^ output[3];
    }
    free(realKey);
    return sum;
}

//Create a new Bloom Filter of a given length and has function.

static inline bloomF *newBF(uint32_t l, uint32_t b[])
{
    struct bloomF *thisBF;
    thisBF = (bloomF *) malloc(sizeof(bloomF));
    thisBF->v = (uint8_t *) calloc((l+7)/8, sizeof(uint8_t));
    thisBF->l = l;
    for (int i = 0; i < 4; i++)
    {
        thisBF->s[i] = b[i];
    }
    return thisBF;
}

//Delete a Bloom filter
static inline void delBF(bloomF *thisBF)
{
    free(thisBF->v);
    free(thisBF);
    return;
}

//Return the value of position k in the Bloom filter
static inline uint32_t valBF(bloomF *x, uint32_t k)
{
    uint32_t d = k/8;
    uint32_t r = k % 8;
    uint8_t mask = 1 << r; //use a mask to keep track of which bit you want
    uint8_t maskedNum = x->v[d] & mask;
    uint8_t theBit = maskedNum >> r;
    return theBit;
}

//Find the length of the Bloom filter
static inline uint32_t lenBF(bloomF *x)
{
    return x->l;
}

//Count bits set to 1 in the Bloom filter
static inline uint32_t countBF(bloomF *x)
{
    uint32_t bitCount = 0;
    uint8_t bit = 0;
    for (uint32_t i = 0; i < x->l; i++)
    {
        bit = valBF(x, i);
        if (bit == 1)
        {
            bitCount++;
        }
    }
    return bitCount;
}

//Set an entry in the Bloom filter
static inline void setBF(bloomF *x, char *key)
{
    uint32_t hashed = 0;
    hashed = hashBF(x, key);
    hashed = hashed % x->l;
    uint32_t d = hashed/8;  //byte/index to set
    uint32_t r = hashed % 8;  //bit position to set
    uint8_t flag = 1;
    flag = flag << r;
    x->v[d] = x->v[d] | flag;
    return;
}

//Clear an entry in the Bloom filter
static inline void clrBF(bloomF *x, char *key)
{
    uint32_t hashed = 0;
    hashed = hashBF(x, key);
    uint32_t d = hashed/8;  //byte to clear
    uint32_t r = hashed % 8;  //bit position to clear
    uint8_t flag = 1;
    flag = ~(flag << r);
    x->v[d] = x->v[d] & flag;
    return;
}

//Check membership in the Bloom filter
static inline uint32_t memBF(bloomF *x, char *key)
{
    uint32_t hashed = 0;
    hashed = hashBF(x, key);
    hashed = hashed % x->l;
    uint32_t bit = valBF(x, hashed);
    return bit;
}

//Print the Bloom filter
static inline void printBF(bloomF *x)
{
    uint32_t bit = 0;
    for (uint32_t i = 0; i < x->l; i++)
    {
        bit = valBF(x, i);
        printf("%u", bit);
    }
    return;
}

# endif
