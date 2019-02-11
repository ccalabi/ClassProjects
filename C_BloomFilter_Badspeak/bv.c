//bv.c: written by Cori Calabi
//This program provides functionality to a bit vector: creating and deleting a bit vector, 
//finding the length of the vector,setting all the bits to 1, setting a specific bit to 1, 
//clearing a bit, and finding the value of a bit

# ifndef _BVector
# define _BVector
# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include "bv.h"

typedef struct bitV { //this is a way of declaring the char and int
	uint8_t *v;  //a pointer, has 8 bits, representing 255 different #s
	uint32_t l;  //has 32  bits, representing over 4 billion different #s
} bitV;

bitV *newVec(uint32_t n)
{  //make a new bitV, more specifically a pointer to the memory of the new bitV
   struct bitV *thisVec;
   thisVec = (bitV *) malloc(sizeof(bitV));
   thisVec->v = (uint8_t *) calloc((n+7)/8, sizeof(uint8_t));
   thisVec->l = n;
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
//   for (uint32_t y = 0; y<d; y++) {
  //    printf("%hhu This is the number after bit set\n", thisVec->v[y]);
   //}
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
