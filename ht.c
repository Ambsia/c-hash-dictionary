// A hash table using open addressing.  Note, that specialised versions could be made more
// efficient.  For example, the key extraction function could be a macro and the hash
// functions could be macros too (or inline functions).  Furthermore, in this file we have
// combined all three open-addressing methods and used a flag and a switch statement to
// select the appropriate strategy.  A more efficient version would be used in real-life
// that would be tailored for the application and, hence, would not repeatedly use clock
// cycles looking up which strategy was to be used.  We have presented them together here
// primarily for educational purposes - so that the single file may be used to create
// different hash tables using different strategies and so that the implementations may be
// viewed together - highlighting the differences as well as the similarities.
// Author: drs

/* I have modified this implementation of a hash table, I have removed quadratic
 * and linear probing. Double hashing is being used as the hashing algorithm, I 
 * have removed the functionality of passing the hash functions and created
 * macros to do this for me. I have also removed passing the max table size as a parameter
 * and set it has a constant in ht.h. I have also removed all other functionality other than
 * inserting, deleting and looking up an item considering the implementation of 
 * dictionary.h did not require it.
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ht.h"

#define HASHFUNONE(key) key % TABLE_SIZE
#define HASHFUNTWO(key) key % 31   

struct ht_implementation
{
    any * items;      // array of stored objects
    any deleted;      // static address reserved to indicate deleted locations
    int size;         // current number of objects stored in the table
    getkeyfun getkey; // user-supplied function to extract a key from an object
    int max;          // user-supplied maximum table size
};

static any deleted;   // <- technique used in Kyle Loudon's Mastering Algorithms with C
                      // (O'Reilly, 1999)

ht * new_ht(getkeyfun getkey)
{
    int i;
    ht * t = (ht*)malloc(sizeof(ht));
    t->deleted = &deleted;
    t->items = (any*)malloc(TABLE_SIZE * sizeof(any));
    t->size = 0;
    t->getkey = getkey;
    t->max = TABLE_SIZE;
    for (i=0; i<t->max; i++)
        t->items[i] = NULL;
    return t;
}

int ht_insert(ht *t, any x)
{
    assert(t!=NULL);
    int i=0;
    int j;
    int k = t->getkey(x);
    for(i=0; i < t->max; i++) {
          j = (HASHFUNONE(k) + (i * HASHFUNTWO(k)))                % t->max;
        if (t->items[j] == NULL || t->items[j] == t->deleted) {
            t->items[j] = x;
            (t->size)++;
            return TRUE;                   // report successful insertion
        }
        else  // keep looking
            continue;
    }
    return FALSE;                           // report failure to insert item
}

int ht_delete(ht *t, int k)
{
    assert(t!=NULL);
    int i, j;
        for(i=0; i < t->max; i++) {
              j = (HASHFUNONE(k) + (i * HASHFUNTWO(k)))                % t->max;
            if (t->items[j] == NULL)        // item was not there
                return FALSE;                   // report failure to delete item
            else if (t->items[j] == t->deleted)
                continue;
            else if (k == t->getkey(t->items[j])) {
                t->items[j] = t->deleted;
                (t->size)--;
                return TRUE;                   // report successful deletion
            }
            else // keep looking
                continue;
        }
}

any ht_lookup(ht *t, int k)
{
    assert(t!=NULL);
    int j, i;
    for(i=0; i < t->max; i++) {
          j = (HASHFUNONE(k) + (i * HASHFUNTWO(k)))                % t->max;
        if (t->items[j] == NULL)
            return NULL;
        else if (t->items[j] == t->deleted)
            continue;
        else if (k == t->getkey(t->items[j])) 
            return t->items[j];         // return the located item
        else // keep looking
            continue;
    }
    return NULL;                        // report failed lookup
}
