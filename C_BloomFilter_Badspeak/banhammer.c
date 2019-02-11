// banhammer.c: Written by Cori Calabi
// This program handles the files hash.c, bf.h, ll.c, and aes.c 
// and the functions contained in them and reads in the 
// badspeak and oldspeak words and controls the bloom filters and hash table


# include <stdio.h>  //for printf and scanf
# include <stdint.h>  //for uint8_t and uint32_t
# include <errno.h>  //for error messages
# include <ctype.h>
# include <string.h>  //for strcmp and such
# include <getopt.h> //for getopt()
# include <stdbool.h>  //for bool

# include "bf.h"
# include "hash.h"
# include "aes.h"
# include "ll.h"

int yylex(void);

extern char* yytext;

//extern bool moveToFront;

bool moveToFront = false;
//int seeks = 0;  //number of findLL iterations
//int callLL = 0;  //number of calls to findLL

int main(int argc, char **argv)
{
    int c;

    //below are the default values for the program, can be changed with command line flags
    uint32_t lengthOne = 1048576; //length can be set, is 2^20 by default
    uint32_t lengthTwo = 1048576;
    uint32_t sizeHT = 10000;  //size can be set, default is 10000
    
    bool printStats = false;
    bool esperanto = false;
   // moveToFront = true;

    while ((c = getopt(argc, argv, "sh:f:mbe")) != -1) 
    {
        switch (c)
        {
            case 's': //don't print output, only statistics
                printStats = true;
                break;
            case 'h':  //change size of hash table
                sizeHT = atoi(optarg);
                break;
            case 'f':  //change size of bloom filter
                lengthOne = atoi(optarg);
                lengthTwo = lengthOne;
                break;
            case 'm':  //use the move to front rule
                moveToFront = true;
                break;
            case 'b':  //do not use the move to front rule
                moveToFront = false;
                break;
            case 'e':  //use Esperanto.txt in addition to newspeak.txt
                esperanto = true;
                break;
        }
    }

    // make the BFs and HT with the correct values
    bloomF *firstBF = newBF(lengthOne, initA);
    bloomF *secondBF = newBF(lengthTwo, initB);
    hashTable *hashT = newHT(sizeHT, initH);

    //variables for statistics
    int dictionary = 0;  //number of words in badspeak.txt
    int translations = 0; //number of translations in newspeak.txt
    int textIN = -1; //number of words read from stdin with yylex()
        //textIN starts at -1 because yylex() reads that EOF character and 
        //textIN is incremented even though EOF is not a word
    float densityOne = 0;  //density of BF 1
    float densityTwo = 0;  //density of BF 2

    /* Read in the bad words and enter them in the bloom filter */
    int closed;
    FILE* badFile = fopen("/afs/cats.ucsc.edu/users/g/darrell/badspeak.txt","r");

    char badStrArr[16];
    char *badStrP;
    badStrP = badStrArr;
    int strLen;

    char oldSpeakArr[32];
    char *oldSpeakP;
    oldSpeakP = oldSpeakArr;

    char newSpeakArr[32];
    char *newSpeakP;
    newSpeakP = newSpeakArr;

    uint32_t counter = 0;
    int read;
    read = fscanf(badFile, "%s \n", badStrP);   
    
    while (read != EOF)
    {
        strLen = strlen(badStrP);
        for (int h = 0; h <= strLen; h++)
        {
            badStrP[h] = tolower(badStrP[h]);
        }
        for (strLen += 1; strLen < 16; strLen++)
        {
            badStrP[strLen] = '\0';
        }
      //  printf("%s\n", badStrP);
        setBF(firstBF, badStrP);
        setBF(secondBF, badStrP);
        insertHT(hashT, badStrP, (char *) NIL);
        read = fscanf(badFile, "%s \n", badStrP);
        dictionary++;
        counter++;
    }

    closed = fclose(badFile);
    if (closed == -1)
    {
        perror(argv[0]);
        exit(errno);
    }

    // Scan in the newspeak file
    FILE* speakFile = fopen("/afs/cats.ucsc.edu/users/g/darrell/newspeak.txt", "r");
    read = fscanf(speakFile, "%s %s \n", oldSpeakP, newSpeakP);
    
    while (read != EOF)
    {
//        printf("%s %s\n", oldSpeakP, newSpeakP);
        strLen = strlen(oldSpeakP);
        for (int j = 0; j <= strLen; j++)
        {
            oldSpeakP[j] = tolower(oldSpeakP[j]);
        }

        for (strLen += 1; strLen < 32; strLen++)
        {
            oldSpeakP[strLen] = '\0';
        }

        strLen = strlen(newSpeakP);
        for (int k = 0; k <= strLen; k++)
        {
            newSpeakP[k] = tolower(newSpeakP[k]);
        }
        for (strLen += 1; strLen < 32; strLen++)
        {
            newSpeakP[strLen] = '\0';
        }

        setBF(firstBF, oldSpeakP);
        setBF(secondBF, oldSpeakP);
        insertHT(hashT, oldSpeakP, newSpeakP);
        read = fscanf(speakFile, "%s %s \n", oldSpeakP, newSpeakP);
        translations++;
    }
    closed = fclose(speakFile);
    if (closed == -1)
    {
        perror(argv[0]);
        exit(errno);
    }

    if (esperanto == true)
    {
        FILE* eFile = fopen("/afs/cats.ucsc.edu/users/g/darrell/Esperanto.txt", "r");
        read = fscanf(eFile, "%s %s \n", oldSpeakP, newSpeakP);
        while (read != EOF)
        {
            strLen = strlen(oldSpeakP);
            for (int b = 0; b <= strLen; b++)
            {
                oldSpeakP[b] = tolower(oldSpeakP[b]);
            }
            for (strLen += 1; strLen < 32; strLen++)
            {
                oldSpeakP[strLen] = '\0';
            }
            strLen = strlen(newSpeakP);
            for (int c = 0; c <= strLen; c++)
            {
                newSpeakP[c] = tolower(newSpeakP[c]);
            }
            for (strLen += 1; strLen < 32; strLen++)
            {
                newSpeakP[strLen] = '\0';
            }
            setBF(firstBF, oldSpeakP);
            setBF(secondBF, oldSpeakP);
            insertHT(hashT, oldSpeakP, newSpeakP);
            read = fscanf(eFile, "%s %s \n", oldSpeakP, newSpeakP);
        }
        closed = fclose(eFile);
        if (closed == -1)
        {
            perror(argv[0]);
            exit(errno);
        }
    }

   // printf("Hash Table\n");
   // printHT(hashT);

    listNode *matchLN;

    int tokenID = 0;
    uint32_t memBFOne = 0;
    uint32_t memBFTwo = 0;

    listNode **matches;
    uint32_t matchSize = 100;
    uint32_t matchIndex = 0;
    matches = (listNode **) calloc(matchSize, sizeof(listNode *));
   
    listNode **badWords;
    uint32_t badSize = 100;
    uint32_t badIndex = 0;
    badWords = (listNode **) calloc(badSize, sizeof(listNode *));
    uint32_t badCounter = 0;

    while (tokenID != -1) //While there is more to scan (before EOF)
    {
        char * text = (char *) malloc(32);
        tokenID = yylex(); //Get a token from the scanner
        textIN++;
        strcpy(text, yytext); //Copy string to text
        strLen = strlen(text);
        for (int n = 0; n <= strLen; n++)
        {
            text[n] = tolower(text[n]); //make the string all lowercase
        }
        for (strLen += 1; strLen < 32; strLen++)
        {
            text[strLen] = '\0';  //pad the string with nulls
        }
//        printf("(%d - %s)", tokenID, text);

        memBFOne = memBF(firstBF, text); //check if the word is in BF 1
        if (memBFOne == 1)
        {
            memBFTwo = memBF(secondBF, text); //if word is in BF 1, check if in BF 2
//            printf("\nmemBFOne: %u, memBFTwo: %u\n", memBFOne, memBFTwo);
            if (memBFTwo == 1)
            {
               if (matchIndex == matchSize)
               {
                   matchSize = matchSize * 2;
                   matches = (listNode **) realloc(matches, sizeof(listNode *) * matchSize);
               }
                matchLN = findHT(hashT,text);  //if word in both BFs, find in Hash table
//                printf("ListNode newspeak: %s", matchLN->newspeak);
                matches[matchIndex] = matchLN;  //add the list node to an array
                if (matchIndex == 0)
                {
                    badWords[matchIndex] = matchLN;
                    badIndex++;
                }
                for (uint32_t a = 0; a < badIndex; a++)
                {
                    if (strcmp(matchLN->oldspeak, badWords[a]->oldspeak) != 0)
                    {
                        badCounter++;
                    }
                }
                if (badCounter == badIndex)
                {
                    if (badIndex == badSize)
                    {
                        badSize = badSize * 2;
                        badWords = (listNode **) realloc(badWords, sizeof(listNode *) * badSize);
                    }
                    badWords[badIndex] = matchLN;
                    badIndex++;
                }    
                badCounter = 0;
                matchIndex++;
            }
        }
        else 
        {
            memBFTwo = 0;
    //        printf("memBFOne: %u\n", memBFOne);
        }
        free(text);
    }

   // printf("Hash Table\n");
   // printHT(hashT);

    int badCount = 0;
    int newCount = 0;
    for (int m = (int) badIndex-1; m >= 0; m--)
    {
        if (badWords[m]->newspeak == (char *) NIL)
        {
            if ((badCount < 1) && (printStats == false))
            {   //Dear Comrade messages from Darrell Long
                printf("Dear Comrade,\n\n");
                printf("You have chosen to use degenerate words that may cause hurt\n");
                printf("feelings or cause your comrades to think unpleasant thoughts.\n");
                printf("This is doubleplus bad. To correct your wrongthink and\n");
                printf("save community consensus we will be sending you to joycamp\n");
                printf("administered by Miniluv.\n");
                printf("\nYour errors:\n\n");
            }
            if (printStats == false)
            {
                printLL(badWords[m]);
            }
            badCount++;
        }
    }

    for (int n = (int) badIndex-1; n >= 0; n--)
    {
        if (badWords[n]->newspeak != (char *) NIL)
        {
            if((newCount < 1) && (badCount == 0) && (printStats == false))
            {
                printf("Dear Comrade,\n\n");
                printf("Submitting your text helps to preserve feelings and prevent\n");
                printf("badthink. Some of the words that you used are not goodspeak.\n");
                printf("The list shows how to turn the oldspeak words into newspeak.\n\n");
            }
            else if ((newCount < 1) && (badCount != 0) && (printStats == false))
            {
                printf("\nThink on these words during your vacation!\n\n");
            }
            newCount++;
            if (printStats == false)
            {
                printLL(badWords[n]);
            }
        }
    }
    
    //If the -s flag was used do this:
    if (printStats == true)
    {
        float average = ((float) seeks)/((float) callLL);
        densityOne = ((float)countBF(firstBF))/((float)lenBF(firstBF));
        densityTwo = ((float)countBF(secondBF))/((float)lenBF(secondBF));
        printf("Seeks %d, Average %.6f, Dictionary %d, Translations ", seeks, average, dictionary);
        printf("%d, Text %d, Densities: %.6f, %.6f\n", translations, textIN, densityOne, densityTwo);
    }

    free(matches);
    free(badWords);
    delBF(firstBF);
    delBF(secondBF);
    delHT(hashT);
    return 0;
}
