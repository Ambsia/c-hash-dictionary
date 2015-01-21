#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dictionary.h"
#include "any.h"
#include "ht.h"

ht * table;

// This struct is the implementation of a dictionary item, containing a word and a description.
struct dictionary_item {
    char word[MAX_WORD_SIZE];
    char description[MAX_DESC_SIZE];
};

/* This function takes a dictionary item extracts the word and then
 * strips each of the characters from the word, mupltiplys them by a prime number
 * and then mods each value by a prime number, then appends the value to h which is later returned as key.
*/
int get_key(any d_item) {
    char word[MAX_WORD_SIZE];
    strcpy(word, ((struct dictionary_item*)d_item)->word);

    int key, h = 0, off = 0, i;
    for(i=0;word[i]!='\0';i++) 
         h = h * 3 + word[off++] % 31;
    key = h;  

    return key;
}

// This function initialises the hash table and passes the get_key function.
void d_initialise() {
    table = new_ht(get_key);
}

/* This function will read each line of the file. If more than 1 argument is found, a dictionary item will be created,
 * then the word and description will be set to the ones found in the text file. The item will then be
 * inserted into the dictionary (hash table).
*/
int d_read_from_file(const char * filename) { 
    FILE *fp;

    char word[MAX_WORD_SIZE];
    char description[MAX_DESC_SIZE];
    struct dictionary_item * d;
    fp = fopen(filename, "r");

	if (fp != NULL) {
	    while (fscanf(fp, "%s %[^\n]", &word, &description) > 1) {
            d = (struct dictionary_item *) malloc(sizeof(struct dictionary_item));
            strcpy(d->word, word);
            strcpy(d->description, description);
            if (ht_lookup(table, get_key(d)) == NULL) {
                if (ht_insert(table,d) != TRUE) {   
                     printf("Item %s couldnt be added into the dictionary.\n", word);    
                } 
            } else {
                if(ht_delete(table, get_key(d)) == TRUE &&  ht_insert(table,d) != TRUE) {
                    printf("Item %s field to be deleted and inserted into the dictionary.\n", word); 
                }
            }
	    }
	    else {
        	return TRUE;
        	fclose(fp);
  	 	}	
	}
    else
        return FALSE;
}

/* This function will create a dictionary item, set the dictionary item's word to 
 * the word passed through the parameter. The dictionary item will then be looked up
 * in the hash table, if the item is found the meaning is modified to the returned
 * items description.
*/
int d_lookup(const char * word, char * meaning) {
    struct dictionary_item * d;
    d = (struct dictionary_item *) malloc(sizeof(struct dictionary_item));
    strcpy(d->word,word);

    if (ht_lookup(table, get_key(d)) != NULL) { 
       d = ht_lookup(table, get_key(d));
       strcpy(meaning, d->description); 
       return TRUE;
    } else {
       return FALSE;
    }
}
