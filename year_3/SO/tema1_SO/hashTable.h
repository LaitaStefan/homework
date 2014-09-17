/*
	Papa Florin, 334CA
	Tema 1 SO
*/

#ifndef HASHTABLE_H_
#define HASHTABLE_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"
#include "linkedList.h"

typedef struct hashTable hashTable;

struct hashTable {
	unsigned int size;
	linkedList **content;
};

//initialize hashtable
hashTable* init_hash(unsigned int size);

//Add entry to hashtable
void add_hash(char *value, hashTable *h);

//Remove entry from hashtable
void remove_entry(char *value, hashTable *h);

//clear hashTable
void clear_hash(hashTable *h);

//Find word in hashtable
void find_word(char *target, hashTable *h, char *file);

//Print bucket
void print_bucket(hashTable *h, unsigned int index, char *file);

//Print hashtable
void print_hash(hashTable *h, char *file);

//Double dimension
hashTable* resize_double(hashTable *h);

//Halve dimension
hashTable* resize_halve(hashTable *h);

//free memory
void free_hash(hashTable *h);

#endif