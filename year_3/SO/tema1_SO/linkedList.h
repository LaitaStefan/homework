/* 
	Papa Florin, 334CA
   	Tema 1 SO
*/

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct linkedList linkedList;

struct linkedList {
	char* value;
	linkedList *prev;
	linkedList *next;
};

//Test existing
int contains(char *target, linkedList *list);

//Initialize list
linkedList* init();

//Remove entry in list
linkedList* remove_elem(char *target, linkedList *list);

//Clear list
linkedList* clear(linkedList *list);

//Add element
void add(char *element, linkedList *list);

//show elements
void print_list(linkedList *list);

#endif