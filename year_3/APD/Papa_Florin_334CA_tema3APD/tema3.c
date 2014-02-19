//Papa Florin, 334CA

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "mpi.h"

#define NUM_COLORS 256

void write_to_file(char *file, int **color, int lines, int cols) {
    int i, j;
    FILE *f = fopen(file, "w");

    fprintf(f, "P2\n");
    fprintf(f, "%d %d\n", cols, lines);
    fprintf(f, "255\n");
    for(i = lines - 1; i >= 0; i--) {
        for(j = 0; j < cols; j++) {
            fprintf(f, "%d ", color[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);
}

void square(double z[]) {
    double a = z[0], b = z[1];
    z[0] = a*a - b*b;
    z[1] = 2*a*b;
}

double value(double z[]) {
    return sqrt(z[0]*z[0] + z[1]*z[1]);
}

void initializare(double info[], double type, double xmin, int cols,
                    double rez, int maxSteps, double cx, double cy) {
    info[0] = type;
    info[1] = xmin;
    info[2] = cols;
    info[5] = rez;
    info[6] = maxSteps; 
    info[7] = cx;
    info[8] = cy;
}

int main(int argc, char** argv) {

    double info[9], xmin, xmax, ymin, ymax, cx, cy, auxCol, rez, z[2];
    int **localColors, **officialColors;
    int lines, cols, chunk, i, j, maxSteps, type, step, total = 0, numLines, maxL, maxC;

    int rank, size;
    MPI_Status status;
    FILE *inFile, *outFile;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if(rank == 0) {
    	inFile = fopen(argv[1], "r");
    	fscanf(inFile, "%d %lf %lf", &type, &xmin, &xmax);
    	fscanf(inFile, "%lf %lf %lf %d", &ymin, &ymax, &rez, &maxSteps);

        if(type == 1) {
            fscanf(inFile, "%lf %lf", &cx, &cy);
        }
        fclose(inFile);

        //numar de linii matrice
        auxCol = (ymax - ymin) / rez;
        lines = round(auxCol);

        //numar de coloane matrice
        auxCol = (xmax - xmin) / rez;
        cols = round(auxCol);

        chunk = lines / size;

        //initializare mesaje vecini
        initializare(info, type, xmin, cols, rez, maxSteps, cx, cy);

        total += chunk;
        for(i = 1; i < size; i++) {
            info[3] = ymin + chunk * i * rez;
            if(i == size - 1) info[4] = lines - total;    //numar linii
            else info[4]= chunk;
            MPI_Send(info, 9, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
            total += chunk;
        }

        total = info[4];

        //rank 0
        info[3] = ymin;
        info[4] = chunk;

        officialColors = (int**)malloc(lines * sizeof(int*));
        for(i = 0; i < lines; i++) {
            officialColors[i] = (int*)malloc(cols * sizeof(int));
        }
    }
    else {
        MPI_Recv(info, 9, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);

        localColors = (int**)malloc((int)info[4] * sizeof(int*));
        for(i = 0; i < (int)info[4]; i++) {
            localColors[i] = (int*)malloc((int)info[2] * sizeof(int));
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    maxL = info[4];
    maxC = info[2];

    for(i = 0; i < maxL; i++) { //variez y
        for(j = 0; j < maxC; j++) { //variez x
            if((int)info[0] == 0) {
                z[0] = 0;
                z[1] = 0;
            }
            else {
                z[0] = info[1] + j*info[5];
                z[1] = info[3] + i*info[5];
            }
            step = 0;

            while(value(z) < 2 && step < (int)info[6]) {
                square(z);
                if((int)info[0] == 0) {
                    z[0] += info[1] + j*info[5];
                    z[1] += info[3] + i*info[5];
                }
                else {
                    z[0] += info[7];
                    z[1] += info[8];
                }  
                step++;
            }

            if(rank == 0) {
                officialColors[i][j] = step % NUM_COLORS;
            }
            else localColors[i][j] = step % NUM_COLORS;
        }
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if(rank != 0) {
        for(i = 0; i < (int)info[4]; i++) {
            MPI_Send(localColors[i], (int)info[2], MPI_INT, 0, 1, MPI_COMM_WORLD);
        }
    }
    else {
        for(i = 1; i < size; i++) {
            if(i == size - 1) numLines = total;
            else numLines = info[4];

            for(j = 0; j < numLines; j++) {
                MPI_Recv(officialColors[i * chunk + j], (int)info[2], MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            }
        }
        write_to_file(argv[2], officialColors, lines, cols);
    }

    if(rank == 0) {
        for(i = 0; i < lines; i++) {
            free(officialColors[i]);
        }
        free(officialColors);
    }          
    else {
        for(i = 0; i < info[4]; i++) {
            free(localColors[i]);
        }
        free(localColors);
    }
    
    MPI_Finalize();	
}


