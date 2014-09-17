/*
	Papa Florin, 334CA
	Tema 1 SO
*/

#include "linkedList.h"
#include "hashTable.h"

//function that interprets the input
void interpret(hashTable **h, char *input) {
	char *tok;
	char *target;
	unsigned int index;

	//printf("running command %s\n", input);

	tok = strtok(input, " \n\r\t");
	if(tok != NULL) {
		if(strcmp(tok, "add") == 0) {
			tok = strtok(NULL, " \n\r\t");
			if(tok != NULL && strlen(tok) > 0) {
				add_hash(tok, *h);
			}
		}
		else if(strcmp(tok, "remove") == 0) {
			tok = strtok(NULL, " \n\r\t");
			if(tok != NULL && strlen(tok) > 0) {
				remove_entry(tok, *h);
			}
		}
		else if(strcmp(tok, "clear") == 0) {
			clear_hash(*h);
		}
		else if(strcmp(tok, "find") == 0) {
			tok = strtok(NULL, " \n\r\t");
			if(tok != NULL && strlen(tok) != 0) {
				target = (char*)malloc(strlen(tok) + 1);
				memcpy(target, tok, strlen(tok));
				tok = strtok(NULL, " ");
				find_word(target, *h, tok);
				free(target);
			}
		}
		else if(strcmp(tok, "print_bucket") == 0) {
			tok = strtok(NULL, " \n\r\t");
			if(tok != NULL && strlen(tok) > 0) {
				index = atoi(tok);
				tok = strtok(NULL, " \n\r\t");
				print_bucket(*h, index, tok);
			}
		}
		else if(strcmp(tok, "print") == 0) {
			tok = strtok(NULL, " \n\r\t");
			print_hash(*h, tok);
		}
		else if(strcmp(tok, "resize") == 0) {
			tok = strtok(NULL, " \n\r\t");
			if(tok != NULL && strlen(tok) > 0) {
				if(strcmp(tok, "double") == 0) {
					*h = resize_double(*h);
				}
				else if(strcmp(tok, "halve") == 0) {
					*h = resize_halve(*h);
				}
			}
		}
	}
}


int main(int argc, char** argv) {
	FILE **f;
	unsigned int size;
	char read[20000];
	hashTable *h;
	int i;

	f = (FILE**)malloc((argc - 2) * sizeof(FILE*));
	if(f == NULL) {
		printf("Nu s-a putut aloca memorie\n");
		return 1;
	}
	size = atoi(argv[1]);
	h = init_hash(size);

	if(argc > 2) {	//avem fisiere de input
		for(i = 2; i < argc; i++) {
			f[i] = fopen(argv[i], "r");
			if(f[i] != NULL) {
				memset(read, 0, 20000);

				while(fgets(read, 20000, f[i])) {
					interpret(&h, read);
					memset(read, 0, 20000);
				}
				//inchidere fisier
				fclose(f[i]);
			}
		}
	}
	else {	//citire de la stdin
		while(fgets(read, 20000, stdin)) {
			interpret(&h, read);
			memset(read, 0, 20000);
		}
	}

	return 0;
}