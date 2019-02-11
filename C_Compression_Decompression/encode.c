// encode.c: This is the encoder for assignment4 
// Author: Cori Calabi (ccalabi@ucsc.edu)

# include <stdio.h>
# include <unistd.h>
# include <stdint.h>
# include <stdlib.h>
# include <fcntl.h>
# include <errno.h>
# include <ctype.h>

# include "queue.h"
# include "huffman.h"
# include "code.h"
# include "is.h"
# include "stack.h"
# include "encode.h"
# include "bitV.h"

# define STD_OUT 1

// Make the codes for each character in the tree
void makeCodes(treeNode *node, code *table, code aCode)
{
    bool check = false;
    code *nCode = &aCode;

    if (node == NIL)
    {
        return;
    }
    if (node->leaf == false/*(node->left != NIL) && (node->right != NIL)*/)
    {
        check = pushCode(nCode, LEFT);
        if (check != true)
        {
            printf("pushCode problem.\n");
        }
        makeCodes(node->left, table, aCode);
	uint32_t  k = 0;
        check = popCode(nCode, &k);
        check = pushCode(nCode, RIGHT);
        makeCodes(node->right, table, aCode);
    }
    else if ((node->left == NIL) && (node->right == NIL))
    {
        buildCode(node, aCode, table);
    }
}


int main(int argc, char **argv)
{
    int c;
    char *inputFile = NULL;
    int fd = 0;
    int bytesToRead = 1;
    int bytesRead = 0;
    uint64_t totalBytes = 0;
    
    char *outputFile = NULL;
    int closed = 0;
    uint64_t charCount[256] = {0};
    uint8_t oneByte = 0;
    bool outFile = false;
    bool printIt = false;
    bool doVerbose = false;

    while ((c = getopt(argc, argv, "i:o:vp")) != -1)
    {
        switch (c)
        {
            case 'i': //read in input file
                inputFile = optarg;
                break;
            case 'o':  //give the name of the output file
                outputFile = optarg;
                outFile = true;
                break;
            case 'v':  // Give statistics
                doVerbose = true;
                break;
            case 'p':  // Print the tree
                printIt = true;
                break;
            case '?':
                //the code in this '?' case is adapted from
                //www.gnu.org/s/libc/manual/html_node/Example-of-Getopt.html
                if (optopt == ('p' || 'r' || 'n'))
                {
                    fprintf(stderr, "Option -%c requires an argument.\n", optopt);
                }
                return 1;
            default:
                fprintf(stderr, "An unknown error occurred.\n");
                return 2;
        }
    }

    //increment elements 0 and 255 of charCount
    charCount[0] = 1;
    charCount[255] = 1;

    fd = open(inputFile, O_RDONLY);
    if (fd == -1)
    {
        printf("Cannot open %s\n", inputFile);
        perror(argv[0]);
        exit(errno);
    }

    bytesRead = read(fd, &oneByte, bytesToRead); //read the file into a buffer
    if (bytesRead == -1)
    {
        perror(argv[0]);
        exit(errno);
    }
 
    //keep reading the file and count the characters until the file ends
    while (bytesRead > 0)
    {
        charCount[oneByte] += 1;
        bytesRead = read(fd, &oneByte, bytesToRead);
        if (bytesRead == -1)
        {
            perror(argv[0]);
            exit(errno);
        }
        totalBytes++;
    }

    closed = close(fd); //close the input file
    if (closed == -1)
    {
        perror(argv[0]);
        exit(errno);
    }

    int arrCount = 0;
    uint32_t qSize = 0;
    uint16_t numLeaves = 0;
    for (int i = 0; i < 256; i++)
    {
        arrCount = arrCount + charCount[i];
        if (charCount[i] > 0)
        {
            qSize++;
            numLeaves++;
        }
    }

    qSize++; // Add 1 to qSize in order to check if queue is full 
    queue *thisQ = newQueue(qSize); 
    bool queued = true; // way to check if the nodes queued
    treeNode *tNode; // pointer to treeNodes

    /* Make treeNodes for each character that appears and
     * put them into the queue */ 
    for (int h = 0; h < 256; h++)
    {
        if ((charCount[h] > 0) && (queued == true))
        {
            tNode = newNode(h, true, charCount[h]);
            queued = enqueue(thisQ, tNode);
            if (queued == false)
            {
                printf("enqueue failed\n");
            }
        }
    }
    thisQ->Q[qSize-1] = NULL;

    // Make the queue a priority queue
    insertionSort(thisQ->Q, qSize - 1);
    thisQ->head = 0;
    thisQ->tail = qSize-1;

    treeNode *deQ = NULL; // two pointers to dequeue nodes into
    treeNode *deQTwo = NULL;
    treeNode *newInternal; // pointer to a new internal node
    bool emptyQ = false; // for exiting the loop
    treeNode *rootNode = NULL; // pointer to the root node

    // Make the tree from the queue 
    // (based on pseudocode from Darrell Long's lecture 5/18/17)
    while (!emptyQ)
    {
        /* until the queue is empty (meaning a full tree
         * has been created), dequeue 2 nodes and join them to 
         * make a new internal node
         */
        deQ = dequeue(thisQ, deQ);
        if (deQ == NULL)
        {
            printf("FAILED\n");
        }

        deQTwo = dequeue(thisQ, deQTwo);
        if (deQTwo == NULL)
        {
            printf("Two FAILED\n");
        }

        if (empty(thisQ))
        {
            emptyQ = true;
        }

        /* If both nodes were successfully dequeued,
         * join them and make a new internal node,
         * then add this new node back into the queue
         * and sort the queue again to make it a priority queue
         */
        if (deQ && deQTwo)
        {
            newInternal = join(deQ, deQTwo);
            bool qD = enqueue(thisQ, newInternal);
            rootNode = newInternal;
          
            if (qD == false)
            {
                printf("New internal node enqueue failed\n");
            }
            // if the internal node was queued:
            else if (qD || (qD && emptyQ))
            {
                insertionSort(thisQ->Q, qSize);
                thisQ->tail = 0;
                int headCounter = 0;
                for (uint32_t n = 0; n < qSize; n++)
                {
                    treeNode *tempP = thisQ->Q[n];
                    if (tempP == NULL)
                    {
                        headCounter++;
                    }
                }
                thisQ->head = headCounter;
            }
        }
    }

    // Print the tree
    if (printIt == true)
    {
    printTree(rootNode, 0);
    printf("\n");
    } 

    // Make an array of codes and fill with 100s (for testing, will be all 0's later)
    code table[256];
    for (int y = 0; y < 256; y++)
    {
          for (int u = 0; u < 256; u++)
          {
              table[y] = newCode();
          }
    }
    code *tableP = &table[0];
    code thisCode = newCode();
    // Make the codes & fill the code table
    makeCodes(rootNode, tableP, thisCode);
    printf("\n");


// Read input file again, find each character in the code table 
// and add the code to the bitvector
    bitV *thisVec = newVec(100000);
    fd = open(inputFile, O_RDONLY);
    if (fd == -1)
    {
        printf("Cannot re-open %s\n", inputFile);
        perror(argv[0]);
        exit(errno);
    }

    int charsRead = 0;
    charsRead = read(fd, &oneByte, bytesToRead);
    if (charsRead == -1)
    {
        perror(argv[0]);
        exit(errno);
    }


    while (charsRead > 0)
    {
        append(thisVec, &table[oneByte]);
        charsRead = read(fd, &oneByte, bytesToRead);
        if (charsRead == -1)
        {
            perror(argv[0]);
            exit(errno);
        }
    }

    closed = close(fd);
    if (closed == -1)
    {
        perror(argv[0]);
        exit(errno);
    }

   
    int bytesWritten = 0;
    uint32_t magicNumber = 0xdeadd00d;
    uint16_t huffSize = (3 * numLeaves) - 1;
    uint64_t bitVSize = (thisVec->p + 7)/8;

    // Write out to a file or to the console
   
    if (outFile == true)
    {
        // To write to the output file
        fd = open(outputFile, O_WRONLY | O_CREAT, 0644);
        if (fd == -1)
        {
            printf("Cannot open: %s\n", outputFile);
            perror(argv[0]);
            exit(errno);
        }

        bytesWritten = write(fd, &magicNumber, 4);
        if (bytesWritten == -1)
        {
            printf("Could not write magic #\n");
            perror(argv[0]);
            exit(errno);
        }
        
        bytesWritten = write(fd, &totalBytes, 8);
        if (bytesWritten == -1)
        {
            printf("Could not input file size\n");
            perror(argv[0]);
            exit(errno);
        }
         
        bytesWritten = write(fd, &huffSize, 2);
        if (bytesWritten == -1)
        {
            printf("Could not input size of Huffman tree\n");
            perror(argv[0]);
            exit(errno);
        }
     
        dumpTree(rootNode, fd);

        bytesWritten = write(fd, &thisVec->v[0], bitVSize);
        if (bytesWritten == -1)
        {
            printf("Could not input bitVector\n");
            perror(argv[0]);
            exit(errno);
        }
       
        closed = close(fd); //close the output file
        if (closed == -1)
        {
            perror(argv[0]);
            exit(errno);
        }
    }
    else 
    {
        bytesWritten = write(STD_OUT, &magicNumber, 4);
            if (bytesWritten == -1)
        {
            printf("Could not input yikes\n");
            perror(argv[0]);
            exit(errno);
        }
       
        bytesWritten = write(STD_OUT, &totalBytes, 8);
            if (bytesWritten == -1)
        {
            printf("Could not input yikes\n");
            perror(argv[0]);
            exit(errno);
        }
        
        bytesWritten = write(STD_OUT, &huffSize, 2);
            if (bytesWritten == -1)
        {
            printf("Could not input yikes\n");
            perror(argv[0]);
            exit(errno);
        }
        
        dumpTree(rootNode, STD_OUT);

        bytesWritten = write(STD_OUT, &thisVec->v[0], bitVSize);
            if (bytesWritten == -1)
        {
            printf("Could not input yikes\n");
            perror(argv[0]);
            exit(errno);
        }
    }

    // Print Statistics
     if (doVerbose == true)
     {
         printf("\nOriginal %lu bits: leaves %u (%u bytes) ", totalBytes * 8, numLeaves, huffSize);
         printf("encoding %lu bits (%2.4lu%%).\n", bitVSize * 8, bitVSize / totalBytes);
     }
                                    

    delVec(thisVec);
    delNode(tNode);
    delTree(rootNode);
    delQueue(thisQ);

    return 0;
}
