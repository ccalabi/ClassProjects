//bv.c: written by Cori Calabi
//This program provides functionality to a bit vector: creating and deleting a bit vector, 
//finding the length of the vector,setting all the bits to 1, setting a specific bit to 1, 
//clearing a bit, and finding the value of a bit

# ifndef _BVector
# define _BVector
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include "bitV.h"
# include "code.h"

typedef struct bitV {
	uint8_t *v;  //a pointer to an array of bits
	uint64_t l;  //the overall length of the bitV in bits
        uint64_t p;  //keeps track of what is the next bit to be set
} bitV;

bitV *newVec(uint32_t n)
{  //make a new bitV, more specifically a pointer to the memory of the new bitV
   struct bitV *thisVec;
   thisVec = (bitV *) calloc(1, sizeof(bitV));
   thisVec->v = (uint8_t *)calloc((n+7)/8, 1);
   thisVec->l = n;
   thisVec->p = 0;
   return thisVec;
}

void delVec(bitV *thisVec)
{  //delete a vector completely, not just clear it
   free(thisVec->v);
   free(thisVec);
   return;
}

void oneVec(bitV *thisVec)
{  //make an array with every bit set to 1
   uint32_t numBytes = (thisVec->l+7)/8;
   for (uint32_t i=0; i<numBytes; i++)
   {
      thisVec->v[i] = 255;
   }
   return;
}

void setBit(bitV *thisVec, uint32_t n)
{  //set a specific bit to 1
   uint32_t d = n/8; //gives the byte/index you want to change 
   uint32_t r = n%8; //gives the bit position you want to change
   uint8_t flag = 1;
   flag = flag << r;
   thisVec->v[d] = thisVec->v[d] | flag;
   return;
}

void clrBit(bitV *thisVec, uint32_t n)
{  //clear the specified bit
   uint32_t d = n/8; //the byte/element you want to clear
   uint32_t r = n%8; //the bit position to be cleared
   uint8_t flag = 1;
   flag = ~(flag << r);
   thisVec->v[d] = thisVec->v[d] & flag;
   return;
}

void append(bitV *thisVec, code *thisCode)
{
    if ((thisVec->p) == thisVec->l)
    {
        thisVec->l = (thisVec->l)*2;
        thisVec->v = (uint8_t *)realloc(thisVec->v, thisVec->l);
    }
    for (uint64_t tBits = 0; tBits < thisCode->l; tBits++)
    {
        uint64_t insertBit = tBits + thisVec->p;
        if (valOneBitC(thisCode, tBits) == 1)
        {
            setBit(thisVec, insertBit);
        }
    }
    thisVec->p += thisCode->l;
    return;
}



uint8_t valBit(bitV *thisVec, uint32_t n)
{  //find the value of a single bit: 0 or 1
   uint32_t d = n/8;
   uint32_t r = n%8;
   uint8_t mask = 1 << r; //use a mask to keep track of which bit you are looking at
   uint8_t maskedNum = thisVec->v[d] & mask;
   uint8_t theBit = maskedNum >> r;
   return theBit;
}

uint32_t lenVec(bitV *thisVec)
{  //simply return the length of the bit vector array in bits
   return thisVec->l;
}

#endif
