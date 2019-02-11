//insertionsort.c: Written by Cori Calabi, based on pseudocode from Darrell Long

# include <stdint.h>
# include "is.h"
# include "queue.h"
# include "huffman.h"

void insertionSort(treeNode **numList, uint32_t length)
{
    static uint32_t counts[] = {0, 0};
    //counts contains number of moves in [0] and number of compares in [1]

    uint64_t array[length];
    for (uint32_t j = 0; j < length; j++)
    {
        treeNode *tempN = numList[j];
        if (tempN == NULL)
        {
            array[j] = 0;
        }
        else
        {
            array[j] = numList[j]->count;
        }
    }

    for (int32_t i = 1; i < (int32_t)length; i++)
    {
        uint64_t temp = array[i];  //store the number of interest
        treeNode* tempItem = numList[i];
        int32_t h = i - 1;
        /*Check if the numbers to the left of the number of interest are bigger than it is, and
         * if they are in order
         */
        while (h >= 0 && array[h] > temp)
        {
            array[h + 1] = array[h];
            numList[h + 1] = numList[h]; //if it is larger, move the bigger number to the right
            counts[0] = counts[0] + 1;
            counts[1] = counts[1] + 1;
            h = h - 1;
        } //keep looking to the left to see if the number of interest is smaller than those
        array[h + 1] = temp;
        numList[h + 1] = tempItem;
        //put the number of interest into the correct element, which depends
        //on if it was larger or smaller than the numbers to its left
        counts[0] = counts[0] + 1;
    }
    return;
}
