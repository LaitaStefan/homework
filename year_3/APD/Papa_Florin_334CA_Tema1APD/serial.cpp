//Papa Florin, 334CA

#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>

int get_max(int *d, int col, int n) {

	int max = 0, index = -1;

	for(int i = 0; i < col; i++) {

		if(d[i] > max && d[i] != n) {

			max = d[i];
			index = i;
		}
	}

	return index;
}

void print_color_numbers(int *col, int n, std::ofstream& out) {

	for(int i = 0; i < n; i ++) {

		out << col[i] << ' ';
	}
	out << '\n';
}

void initializare(int *d, int val, int size) {

	for(int i = 0; i < size; i++) {

		d[i] = val;
	}
}

int main(int argc, char* argv[]) {

	int **senators, n, colors, sapt, *color_nr;
	std::ifstream in(argv[2]);
	std::ofstream out(argv[3]);

	//citire valori din fisierul de input
	in >> n;
	in >> colors;

	color_nr = (int*)malloc(colors * sizeof(int));
	senators = (int**)malloc(n * sizeof(int*));

	for(int i = 0; i < n; i++) {
		senators[i] = (int*)malloc(n * sizeof(int));
		for(int j = 0; j < n; j++) {
			in >> senators[i][j];
		}
	}

	in.close();
	sapt = atoi(argv[1]);

	//for pentru iteratii saptamani
	for(int s = 0; s < sapt; s++) {

		int aux[n][n];	//noua matrice de culori
		memset(color_nr, 0, 4 * colors);

		//2 for-uri pentru a parcurge matricea 
		for(int i = 0; i < n; i ++) {

			for(int j = 0; j < n; j++) {

				int dist[colors];
				initializare(dist, n, colors); //initializare distante cu n

				//2 for-uri pentru a calcula distantele
				for(int k = 0; k < n; k++) {

					for(int l = 0; l < n; l++) {

						int d = std::max(abs(i - k), abs(j - l)); //distanta
																//curenta
						if(d != 0 && d < dist[senators[k][l]]) {

							dist[senators[k][l]] = d;
						}
					}
				}

				aux[i][j] = get_max(dist, colors, n); //intoarce noua culoare
				color_nr[aux[i][j]] ++;
			}
		}

		for(int i = 0; i < n; i++) {
			for(int j = 0; j < n; j++) {
				senators[i][j] = aux[i][j];
			}
		}
		print_color_numbers(color_nr, colors, out);
	}

	//scriere matrice finala in fisier
	for(int i = 0; i < n; i++) {
		for(int j = 0; j < n; j++) {

			out << senators[i][j] << ' ';
		}
		out << '\n';
	}
	out.close();

	//dezalocare matrice senatori
	for(int i = 0; i < n; i++){

		free(senators[i]);
	}
	free(senators);
	free(color_nr);

	return 0;
}