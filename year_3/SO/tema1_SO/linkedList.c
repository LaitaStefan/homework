/*
	Papa Florin, 334CA
	Tema 1 SO
*/

#include "linkedList.h"

linkedList* init() {
	linkedList *new_list = (linkedList*)malloc(sizeof(linkedList));

	if(new_list == NULL) {
		printf("Nu s-a putut aloca o noua lista\n");
		return NULL;
	}
	new_list->value = NULL;
	new_list->prev = NULL;
	new_list->next = NULL;

	return new_list;
}

int contains(char *target, linkedList *list) {
	linkedList *current = list;

	if(current->value == NULL) {
		return 0;
	}
	else {
		do {
			if(strcmp(current->value, target) == 0) {
				return 1;
			}
			current = current->next;
		}
		while(current != NULL);
	}

	return 0;
}

linkedList* remove_elem(char *target, linkedList *list) {
	linkedList *current = list;
	linkedList *head = list;

	while(current != NULL) {

		if(strcmp(current->value, target) == 0) {

			//daca nu este primul element din lista
			if(current->prev != NULL) {
				current->prev->next = current->next;
			}
			else {
				head = current->next;
			}

			//daca nu este ultimul
			if(current->next != NULL) {
				current->next->prev = current->prev;
			}
			
			if(current->value != NULL) free(current->value);
			free(current);
			break;
		}

		current = current->next;
	}

	if(head == NULL) {
		return init();
	}
	else {
		return head;
	}
}

linkedList* clear(linkedList *list) {
	linkedList *current = list;

	do {
		if(current != NULL && current->value != NULL) {
			current = remove_elem(current->value, current);
			//print_list(current);
		}
	}
	while(current != NULL && current->value != NULL);
	return current;
}

void add(char *element, linkedList *list) {
	linkedList *current = list, *new_list;

	if(!contains(element, list)) { 
		if(current->value == NULL) {
			current->value = (char*)malloc((strlen(element) + 1) * sizeof(char));
			if(current->value == NULL) {
				printf("Nu s-a putut aloca memorie\n");
				return;
			}
			strcpy(current->value, element);
		}
		else {

			while(current->next != NULL) {
				current = current->next;
			}

			new_list = init();
			new_list->value = (char*)malloc((strlen(element) + 1) * sizeof(char));
			if(current->value == NULL) {
				printf("Nu s-a putut aloca memorie\n");
				return;
			}
			strcpy(new_list->value, element);
			current->next = new_list;
			new_list->prev = current;
		}
	}
}

void print_list(linkedList *list) {
	linkedList *current = list;
	int len = 0;

	do {
		if(current->value != NULL) {
			printf("%s ", current->value);
			len++;
		}
		current = current->next;
	}
	while(current != NULL);

	printf("%d\n", len);
}