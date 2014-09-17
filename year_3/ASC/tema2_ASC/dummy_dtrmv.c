/*
 * Papa Florin, 334CA
 * Tema 2 ASC
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

void my_dtrmv_dummy(const char UPLO,
					const char TRANS,
					const char DIAG,
					const int N,
					const double *A,
					const int LDA,
					double *X,
					const int INCX) {

	int i, j;
	double *temp = (double*)malloc(N * sizeof(double));
	memcpy(temp, X, N * sizeof(double));

	if(TRANS == 'T' || TRANS == 't' || TRANS == 'C' || TRANS == 'c') {
		for(i = 0; i < N; i++) {
			X[i] = 0;
			for(j = 0; j < N; j++) {
				X[i] += A[N * j + i] * temp[j];
			}
		}	
	}
	else {
		for(i = 0; i < N; i++) {
			X[i] = 0;
			for(j = 0; j < N; j++) {
				X[i] += A[N * i + j] * temp[j];
			}
		}
	}
}

void initialize(double *A, double *X, int N) {
	int i, j;
	
	for(i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			A[i * N + j] = 1;
		}
		X[i] = 1;
	}
}

int main(int argc, char **argv) {
	int i, SIZE;
	double *A, *X;

	SIZE = atoi(argv[1]);

	A = (double*)malloc(SIZE * SIZE * sizeof(double));
	X = (double*)malloc(SIZE * sizeof(double));

	initialize(A, X, SIZE);
	
	struct timeval start, end;
	gettimeofday(&start, NULL);

	my_dtrmv_dummy('U', 'n', 'N', SIZE, A, SIZE, X, 1);

	gettimeofday(&end, NULL);

	float elapsed = ((end.tv_sec - start.tv_sec)*1000000.0f + end.tv_usec - start.tv_usec)/1000000.0f;
	printf("%12f\n", elapsed);

	free(A);
	free(X);	
    	return 0;
}
