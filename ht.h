// A hash table abstract data type
// Author: drs

#ifndef HT_H
#define HT_H

#include "any.h"

#define TABLE_SIZE 2000

#define TRUE 1
#define FALSE 0


typedef struct ht_implementation ht;
typedef int (*getkeyfun)(any a);

ht * new_ht(getkeyfun getkey);
int   ht_insert(ht *t, any x);
int   ht_delete(ht *t, int k);
any   ht_lookup(ht *t, int k);

#endif
