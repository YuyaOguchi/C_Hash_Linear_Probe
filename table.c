/*
 Yuya Oguchi
 COEN12L Lab 3
 table.c
 2/3/15
*/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "set.h"

#define EMPTY 0 // 0 is replaced with Empty to read easier
#define FULL 1  // 1 is replaced with Full to read easier
#define TOMB 2  // 2 is replaced with Tomb to read easier

struct set
{
    int count; //current number of elts(words)
    int length; // max length
    char **elts; // pointer to word
    int *flags;
};
//O(1)
SET *createSet(int maxElts)
{
    int i;
    SET *sp;
    sp = malloc(sizeof(SET));
    assert(sp != NULL); //check to see if sp already exist
    sp -> count = 0; // store 0 as count of elts
    sp -> length = maxElts; // set the max length as max elts
    sp -> elts = malloc(maxElts*sizeof(char*)); // create space for each elt
    sp -> flags = malloc(maxElts*sizeof(int));
    assert(sp-> elts != NULL ); // check to make sure the space is created
    assert(sp-> flags != NULL);
    for (i = 0; maxElts > i; i++)
        sp->flags[i] = EMPTY;
    return sp; // return the set
}
//O(n)
void destroySet(SET *sp)
{
    assert(sp!= NULL);//check to see if sp exist
    int i;
    for (i = 0; sp -> count<i; i++)
    {
        free(sp->elts[i]);// free each elts in the set
    }
    free(sp->flags);
    free(sp->elts);//free pointer of elts
    free(sp);// free elts

}
//O(1)
int numElements(SET *sp)//use the count number stored in the set
{
    return sp->count;//access count
}


unsigned hashString(char *s) { //returns the location to look for
    unsigned hash = 0;
    while (*s != '\0')
        hash = 31 * hash + *s ++;
    return hash;
}


//O(n)
int findElement(SET *sp, char *elt, bool* found) // hash table
{
    *found = false; //the elt is not found unless specified
    unsigned home = hashString(elt) % sp->length; // gives you the home location where you start counting
    int i;

    int finalloc = -1; // final location

    for (i = 0; i < sp->length; i++)
    {
        int pos = (home+i)%sp->length; // current checking position
        if (sp->flags[pos] == FULL) {
            if (strcmp(sp->elts[pos], elt) == 0) { // check to see if this is what you are looking for
                *found = true; // the element is found
                return pos; // return the current location
            }
        }if (sp->flags[pos] == TOMB){ // there was something
            if (finalloc==-1) { // if the tomb was not checked, check it
                finalloc = pos;//this gives the first tomb location
            }
        }if (sp->flags[pos] == EMPTY){
            if (finalloc==-1) { // if the location has not found the tomb, then empty becomes the location
                finalloc = pos; // return location is the current location
            }
            break; // since you already found the location, break out of the loop
        }
    }

    return finalloc; // return the final location
}

//O(n)
bool hasElement(SET *sp, char *elt)
{
    bool found;
    int i = findElement(sp, elt, &found); // check for duplicate

    return found; // return the result

}

//O(n)
bool addElement(SET *sp, char *elt)
{
    bool found;
    int i = findElement(sp, elt, &found);// find element for duplicate

    if (!found)// if there is no duplicate
    {
        sp->elts[i] = strdup(elt); // add new element to set at the end
        sp->flags[i] = FULL; // give the location full flag
        sp->count++;
        return true;
    }
    return false;//if there is duplicate, do not add

}
//O(n)
bool removeElement(SET* sp, char *elt)
{
    bool found;
    int i = findElement(sp, elt, &found);// find element for duplicate
    if(found)// if there is elt, then remove it
    {
    	free(sp->elts[i]); // free the location
        sp->flags[i] = TOMB; // give the location tombstone
        sp->count--;
        return true;
    }

    return false;// no elt matched elts in set.
}
