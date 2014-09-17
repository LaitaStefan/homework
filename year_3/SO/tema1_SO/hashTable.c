/*
	Papa Florin, 334CA
	Tema 1 SO
*/

#include "hashTable.h"

hashTable* init_hash(unsigned int size) {
	int i;
	hashTable *h;

	h = (hashTable*)malloc(sizeof(hashTable));
	if(h == NULL) {
		printf("Nu s-a putut aloca tabela\n");
		return NULL;
	}
	h->size = size;
	h->content = (linkedList**)malloc(size * sizeof(linkedList*));
	if(h->content == NULL) {
		printf("Nu s-a putut aloca lista\n");
	}
	for(i = 0; i < size; i++) {
		h->content[i] = init();
	}

	return h;
}

void add_hash(char *value, hashTable *h) {
	int index;

	index = hash(value, h->size);
	add(value, h->content[index]);
}

void remove_entry(char *value, hashTable *h) {
	int index;

	index = hash(value, h->size);
	h->content[index] = remove_elem(value, h->content[index]);
}

void clear_hash(hashTable *h) {
	int i;

	for(i = 0; i < h->size; i++) {
		h->content[i] = clear(h->content[i]);
	}
}

void find_word(char *target, hashTable *h, char *file) {
	int index, exists;
	FILE *f;

	index = hash(target, h->size);
	exists = contains(target, h->content[index]);
	if(file == NULL) {
		if(exists) {
			printf("True\n");
		}
		else {
			printf("False\n");
		}
	}
	else {
		f = fopen(file, "a");
		if(exists) {
			fprintf(f, "True\n");
		}
		else {
			fprintf(f, "False\n");
		}
		fclose(f);
	}
}

void print_bucket(hashTable *h, unsigned int index, char *file) {
	FILE *f;
	linkedList *l = h->content[index];
	int ref = 0;

	if(file != NULL) {
		f = fopen(file, "a");
	}
	do {
		if(l->value != NULL && strlen(l->value) > 0) {
			if(file != NULL) {
				fprintf(f, "%s ", l->value);
			}
			else {
				printf("%s ", l->value);
			}
			ref++;
		}
		l = l->next;
	}
	while(l != NULL);

	if(file != NULL) {
		if(ref > 0) {
			fprintf(f, "\n");
		}
		fclose(f);
	}
	else {
		if(ref > 0) {
			printf("\n");
		}
	}
}

void print_hash(hashTable *h, char *file) {
	int i;

	for(i = 0; i < h->size; i++) {
		print_bucket(h, i, file);
	}
}

hashTable* resize_double(hashTable *h) {
	int i;
	linkedList *current;
	hashTable *new_h = init_hash(h->size * 2);

	new_h->size = h->size * 2;

	for(i = 0; i < h->size; i++) {
		current = h->content[i];

		do {
			if(current->value != NULL) {
				add_hash(current->value, new_h);
			}
			current = current->next;
		}
		while(current != NULL);

		h->content[i] = clear(h->content[i]);
		free(h->content[i]);
	}

	free(h->content);
	free(h);

	return new_h;
}

hashTable* resize_halve(hashTable *h) {
	int i;
	linkedList *current;
	hashTable *new_h = init_hash(h->size / 2);

	new_h->size = h->size / 2;

	for(i = 0; i < h->size; i++) {
		current = h->content[i];

		do {
			if(current->value != NULL) {
				//printf("##%s##", current->value);
				add_hash(current->value, new_h);
			}
			current = current->next;
		}
		while(current != NULL);

		h->content[i] = clear(h->content[i]);
		free(h->content[i]);
	}

	free(h->content);
	free(h);

	return new_h;
}

void free_hash(hashTable *h) {
	int i;

	for(i = 0; i < h->size; i++) {
		h->content[i] = clear(h->content[i]);
		free(h->content[i]);
	}
	free(h->content);
	free(h);
}