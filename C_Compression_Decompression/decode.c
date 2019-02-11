/*
 * decode.c
 * Author: Ryan Hamamoto
 * CMPS 12B: Sprint 2017
 */

# include <getopt.h>
# include <stdio.h>
# include <stdint.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <stdbool.h>
# include <string.h>

# include "huffman.h"
# include "code.h"
# include "stack.h"

int main(int argc, char *argv[])
{
        extern treeNode ** reference;
	int userOption = 0, outputFlag = 0, printTreeFlag = 0, verboseFlag = 0; //initialize the command line inputs
	opterr = 0;
	char *input, *output; //initialize strings to hold file names
	while ((userOption = getopt(argc, argv, "vpi:o:")) != -1) //take in user input
	{
		switch(userOption)
		{
			case 'i':
			input = optarg; //store the name of the input file
			break;
			
			case 'o':
			output = optarg; //store the name of the output file
			outputFlag = 1; //set the flag to print to file
			break;

			case 'v':
			verboseFlag = 1;
			break;
			
			case 'p':
			printTreeFlag = 1;
			break;

			case '?':
			if(optopt == 'i' || optopt == 'o')
			{
				printf("\nERROR: please enter an argument after the option (-%c)\n", optopt);
			}
			else
			{
				printf("\nERROR: unrecognized input\n");
			}
			break;
		}
	}
	FILE *inputFile = fopen(input, "r"); //open the file in read only mode
	if(inputFile == NIL) //quit if file not opened correctly
	{
		printf("\nERROR: problem opening file\n");
		exit(errno);
	}
	uint32_t magicNumber = 0; //initialize storage integers
	uint64_t fileSize = 0;
	uint16_t treeSize = 0;
	uint8_t byteRead = 0;
	if(fread(&magicNumber, 4, 1, inputFile) == 0) //read in the key
	{
                printf("\nERROR: cannot read file\n");
                exit(errno);
	}
	if(magicNumber != 0XDEADD00D) //if invalid key, exit
	{
		printf("\nERROR: This is not an encrypted file\n");
		exit(errno);
	}
	else
	{
		if(fread(&fileSize, 8, 1, inputFile) == 0) //the next 8 bytes are the file size
		{
			printf("\nERROR: cannot read file\n");
			exit(errno);
		}
		if(fread(&treeSize, 2, 1, inputFile) == 0) //the next 2 bytes are the tree size
		{
			printf("\nERROR: cannot read file\n");
			exit(errno);
		}
		if(verboseFlag) //if user requested statistics
		{
			printf("\nOriginal %lu bits: tree (%u)\n", (fileSize) * 8 , treeSize);
		}
		uint8_t savedTree[treeSize + 1]; //create array of treeSize bytes
		fread(&savedTree, treeSize, 1, inputFile); //read exact size bytes into saved tree
		treeNode *root = loadTree(savedTree, treeSize); //create the huffman tree
		if(printTreeFlag) //prints tree, used for debugging
		{
			printTree(root, 0);
		}
		char outFile[fileSize + 1]; //create an array to store the output
		memset(outFile, 0, fileSize + 1); //initialize all members to 0
        	reference = &root; //initialize reference pointer to root
		int readByte = 1; //initialized to 1 otherwise it will not enter the next loop
		for (;readByte != 0;) //as long as bytes are being read
		{
			readByte = fread(&byteRead, 1, 1, inputFile); //get the first byte of encoded data
			stepTree(root, byteRead, outFile); //and step through the tree
			
		}
		delTree(root); //free all the data in the tree
		if (outputFlag == 0) //unless specified, print to stdout
		{
			for(uint64_t k = 0; k < fileSize; k +=1)
	   		{
				printf("%c", outFile[k]);
			}
		}
		else if(outputFlag == 1)
		{
			FILE *outputFile = fopen(output, "w");
			fwrite(&outFile, 1, fileSize, outputFile);
			fclose(outputFile);
		}
	}
	fclose(inputFile); //close the document
	return 0;
}

