/*
 * Papa Florin, 334CA
 * Tema 2 ASC
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cblas.h>

void initialize(double *a, double *x, int size) {
	int i, j;

	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			a[i * size + j] = 1;
		}
		x[i] = 1;
	}
}

int  main(int argc, char **argv) {
	double *A, *X;
	int SIZE;

	SIZE = atoi(argv[1]);

	A = (double*)malloc(SIZE * SIZE * sizeof(double));
	X = (double*)malloc(SIZE * sizeof(double));

	initialize(A, X, SIZE);
	
	struct timeval start, end;
 	gettimeofday(&start, NULL);

	cblas_dtrmv(CblasRowMajor, CblasUpper, CblasNoTrans, CblasNonUnit, SIZE, A, SIZE, X, 1);

	gettimeofday(&end, NULL);

	float elapsed = ((end.tv_sec - start.tv_sec)*1000000.0f + end.tv_usec - start.tv_usec)/1000000.0f;
	printf("%12f\n", elapsed);
	
	free(A);
	free(X);
    return 0;
}
