//Papa Florin, 334CA

#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <set>
#include <queue>

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

int print_color_numbers(int *col, int n, std::ofstream& f) {
	int nr = 0;

	for(int i = 0; i < n; i ++) {

		f << col[i] << ' ';
		if(col[i] == 0) nr++;
	}
	f << '\n';
	return nr;
}

void initializare(int *d, int val, int size) {

	for(int i = 0; i < size; i++) {

		d[i] = val;
	}
}

int main(int argc, char* argv[]) {

	int n, colors, sapt, elim = 0, *color_nr, **senators;
	std::ifstream in(argv[2]);
	std::ofstream out(argv[3]);

	//citire valori din fisierul de input
	in >> n;
	in >> colors;

	int aux[n][n];
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

		memset(color_nr, 0, 4 * colors);

		//2 for-uri pentru a parcurge matricea 
		for(int i = 0; i < n; i ++) {

			for(int j = 0; j < n; j++) {

				int dist[colors];
				int level = 1;
				int size = 0;
				int finish = 0;

				initializare(dist, n, colors);

				while(size <= colors - elim) {

					int lim11, lim12, lim21, lim22;

					lim11 = std::max(0, j - level);
					lim12 = std::min(j + level, n - 1);

					lim21 = std::max(0, i - level);
					lim22 = std::min(i + level, n - 1);

					//linia de sus si de jos
					if(i - level >= 0 && finish == 0) {
						for(int k = lim11; k <= lim12; k++) {

							if(dist[senators[i - level][k]] == n) {
								dist[senators[i - level][k]] = level;
								size++;	
								if(size == colors - elim) {
									finish = 1;
									break;
								}	
							}
						}
					}

					if(i + level < n && finish == 0) {
						for(int k = lim11; k <= lim12; k++) {
							if(dist[senators[i + level][k]] == n) {
								dist[senators[i + level][k]] = level;
								size++;	
								if(size == colors - elim) {
									finish = 1;
									break;
								}	
							}	
						}
					}	

					//coloanele din stanga si din dreapta
					if(j - level >= 0 && finish == 0) {
						for(int k = lim21; k <= lim22; k++) {
							if(dist[senators[k][j - level]] == n) {
								dist[senators[k][j - level]] = level;
								size++;	
								if(size == colors - elim) {
									finish = 1;
									break;
								}	
							}
						}
					}

					if(j + level < n && finish == 0) {
						for(int k = lim21; k <= lim22; k++) {
							if(dist[senators[k][j + level]] == n) {
								dist[senators[k][j + level]] = level;
								size++;	
								if(size == colors - elim) {
									finish = 1;
									break;
								}	
							}
						}	
					}

					level++;
					if(level == n || finish == 1) break;	
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
		elim = print_color_numbers(color_nr, colors, out); //cate culori 
															//au disparut
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