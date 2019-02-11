//ll.c: Written by Cori Calabi
//Creates and handles linked lists

# ifndef NIL
# define NIL (void *) 0
# endif

# ifndef MTF
# define MTF	true
# endif

# ifndef _LL_H
# define _LL_H
# endif

# include <string.h>
# include <stdbool.h>
# include <stdlib.h>
# include <stdio.h>

# include "ll.h"

typedef struct listNode listNode;

struct listNode
{
    char *oldspeak, *newspeak;
    listNode *next;
};

int seeks = 0;
int callLL = 0;
bool moveToFront;

listNode *newNode(const char *oldW, const char *newW)
{
    listNode *thisNode;
    thisNode = (listNode *) malloc(sizeof(listNode));
    thisNode->oldspeak = strdup(oldW);
    if (newW != NIL)
    {
        thisNode->newspeak = strdup(newW);
    }
    else
    {
        thisNode->newspeak = NIL;
    }
    thisNode->next = NIL;
    return thisNode;
}

void delNode(listNode *node)
{
    free(node->oldspeak);
    free(node->newspeak);
    free(node);
    node = NIL;
    return;
}

void delLL(listNode *head)
{
    listNode *tempPtr;
    while (head != NIL)
    {
        tempPtr = head;
        head = head->next;
        delNode(tempPtr);
    }
    return;
}

//add a new node to the head of the linked list (pass the pointer to the old head)
//Based on pseudocode from Arjun Govindjee
listNode *insertLL(listNode **head, const char *oldW, const char *newW)
{
    listNode *newNd;
    newNd = newNode(oldW, newW);
    newNd->next = *head;
    *head = newNd;
    return newNd;
}

//Find an oldspeak word in the hash table equal to entered string
//This must have both move-to-front functionality and not
listNode *findLL(listNode **head, const char *oldW)
{
    if (*head == NIL)
    {
        return NIL;
    }
    callLL++;
    listNode *match = NIL;
    listNode *prevND = *head;
    listNode *temp = NIL;
    int compResult = 1;

    if (prevND->next != NIL)
    {
        temp = prevND->next;    
        int res = strcmp(prevND->oldspeak, temp->oldspeak);
    
        for (match = *head; match != NIL; match = match->next)
        {
            seeks++;
            compResult = strcmp(match->oldspeak, oldW);
            if (compResult == 0)
            {
                if ((moveToFront == MTF) && (res != 0))
                {
                    seeks++;
                    prevND->next = NIL;//match->next;
                    match->next = *head;
                    *head = match;
                    return *head;
                }
                else
                {
                    return match;
                }
            }
            prevND = match;
        }
    }
    else
    {
        match = *head;
        compResult = strcmp(match->oldspeak, oldW);
        if (compResult == 0)
        {
            return match;
        }
    }
    return NIL;
}

//Print the words in the list
//Based on code from Darrell Long, in Piazza post 542
void printLL(listNode *node)
{
    if (node != NIL)
    {
        listNode *temp = node;
      //  for (listNode *temp = node; temp != NIL; temp = temp->next)
       // {
            if (temp->newspeak != (char *) NIL)
            {
                printf("%s -> %s", temp->oldspeak, temp->newspeak);
            }
            else 
            {
                printf("%s", temp->oldspeak);
            }
      //  }
    
    printf("\n");
    }
    return;
}


//int main(void)
//{

//return 0;
//}
