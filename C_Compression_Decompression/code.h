// Author: Darrell Long
# ifndef _CODE_H
# define _CODE_H

# include <stdint.h>
# include <stdbool.h>
# include <stdio.h>

typedef struct code
{
    uint8_t bits[32];
    uint32_t l;  // position for next bit to be pushed
//    uint32_t p; // current position of bits
} code;

static inline code newCode()
{
    code t;
    for (int i = 0; i < 32; i += 1)
    {
        t.bits[i] = 0;
    }
    t.l = 0;
    return t;
}

// Author of valCode: Cori Calabi
// Gets each bit of a code (for testing purposes)
static inline void valCode(code *c)
{
    uint32_t d = 0;
    uint32_t r = 0;
    uint32_t mask = 0;
    uint8_t maskedNum = 0;
    uint8_t theBit = 0;
    for (uint32_t bit = 0; bit < (c->l); bit++)
    {
        d = bit / 8;
        r = bit % 8;
        mask = 1 << r;
        maskedNum = c->bits[d] & mask;
        theBit = maskedNum >> r;
        printf("%d ", theBit);
    }
    printf("bits");
    return;
}

// Author of valOneBitC: Cori Calabi
// Finds the value of a single bit in a code
static inline uint8_t valOneBitC(code *c, uint64_t thisBit)
{
    uint32_t d = thisBit / 8;
    uint32_t r = thisBit % 8;
    uint32_t mask = 1 << r;
    uint8_t maskedNum = c->bits[d] & mask;
    uint8_t theBit = maskedNum >> r;
    return theBit;
} 

static inline bool pushCode(code *c, uint32_t k)
{
    if (c->l > 256)
    {
        return false;
    }
    else if (k == 0)
    {
        c->bits[c->l / 8] &= ~(0x1 << (c->l % 8));
        c->l += 1;
    }
    else
    {
        c->bits[c->l / 8] |= (0x1 << (c->l % 8));
        c->l += 1;
    }
    return true;
}

static inline bool popCode(code *c, uint32_t *k)
{
    if (c->l == 0)
    {
        return false;
    }
    else
    {
        c->l -= 1;
        *k = ((0x1 << (c->l % 8)) & c->bits[c->l / 8]) >> (c->l % 8);
        return true;
    }
}

static inline bool emptyCode(code *c)
{
    return c->l == 0;
}

static inline bool fullCode(code *c)
{
    return c->l == 256;
}
# endif
