/*
 * Papa Florin, 334CA
 * Tema 4 ASC
 */

/*
 * Copyright 1993-2006 NVIDIA Corporation.  All rights reserved.
 *
 * NOTICE TO USER:
 *
 * This source code is subject to NVIDIA ownership rights under U.S. and
 * international Copyright laws.
 *
 * This software and the information contained herein is PROPRIETARY and
 * CONFIDENTIAL to NVIDIA and is being provided under the terms and
 * conditions of a Non-Disclosure Agreement.  Any reproduction or
 * disclosure to any third party without the express written consent of
 * NVIDIA is prohibited.
 *
 * NVIDIA MAKES NO REPRESENTATION ABOUT THE SUITABILITY OF THIS SOURCE
 * CODE FOR ANY PURPOSE.  IT IS PROVIDED "AS IS" WITHOUT EXPRESS OR
 * IMPLIED WARRANTY OF ANY KIND.  NVIDIA DISCLAIMS ALL WARRANTIES WITH
 * REGARD TO THIS SOURCE CODE, INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, AND FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL NVIDIA BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL,
 * OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS
 * OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
 * OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OR PERFORMANCE OF THIS SOURCE CODE.
 *
 * U.S. Government End Users.  This source code is a "commercial item" as
 * that term is defined at 48 C.F.R. 2.101 (OCT 1995), consisting  of
 * "commercial computer software" and "commercial computer software
 * documentation" as such terms are used in 48 C.F.R. 12.212 (SEPT 1995)
 * and is provided to the U.S. Government only as a commercial end item.
 * Consistent with 48 C.F.R.12.212 and 48 C.F.R. 227.7202-1 through
 * 227.7202-4 (JUNE 1995), all U.S. Government End Users acquire the
 * source code with only those rights set forth herein.
 */

// includes, system
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <helper_cuda.h>
#include <helper_timer.h>
#include <helper_functions.h>
#include <helper_math.h>

// includes, project
#include "2Dconvolution.h"


////////////////////////////////////////////////////////////////////////////////
// declarations, forward

extern "C"
void computeGold(float*, const float*, const float*, unsigned int, unsigned int);

Matrix AllocateDeviceMatrix(int width, int height);
Matrix AllocateMatrix(int width, int height);
void FreeDeviceMatrix(Matrix* M);
void FreeMatrix(Matrix* M);

void ConvolutionOnDevice(const Matrix M, const Matrix N, Matrix P, FILE *f);
void ConvolutionOnDeviceShared(const Matrix M, const Matrix N, Matrix P, FILE *f);

// Get a matrix element
__device__ float GetElement(const Matrix A, int row, int col)
{   
    if(row >= 0 && row < A.height && col >= 0 && col < A.width) {
        return A.elements[row * A.width + col];
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Înmulțirea fără memorie partajată
////////////////////////////////////////////////////////////////////////////////
__global__ void ConvolutionKernel(Matrix M, Matrix N, Matrix P)
{
    //calcul rezultat convoluție
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int i, j;
    int indexP = row * P.width + col;

    if(row < N.height && col < N.width) {
        P.elements[indexP] = 0;
        for(i = 0; i < M.height; i++) {
            for(j = 0; j < M.width; j++) {
                int rowN = row + i - 2;
                int colN = col + j - 2;

                if(rowN >= 0 && rowN < N.height) {
                    if(colN >= 0 && colN < N.width) {
                        int indexM = i * M.width + j;
                        float Melem = M.elements[indexM];
                        float Nelem = N.elements[rowN * N.width + colN];
                        P.elements[indexP] += Melem * Nelem;
                    }
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////
// Înmulțirea cu memorie partajată
////////////////////////////////////////////////////////////////////////////////
__global__ void ConvolutionKernelShared(Matrix M, Matrix N, Matrix P)
{
    int threadCol = blockIdx.x * blockDim.x + threadIdx.x;
    int threadRow = blockIdx.y * blockDim.y + threadIdx.y;
    float Pvalue = 0;
    __shared__ float Ns[SHARED_BLOCK_SIZE][SHARED_BLOCK_SIZE];
    __shared__ float Ms[KERNEL_SIZE][KERNEL_SIZE];
    int row = threadIdx.y;
    int col = threadIdx.x;
    int i, j;

    //calculul rezultatului convoluției

    // Load M and Nsub from device memory to shared memory
    // Each thread loads one element of each sub-matrix
    if(threadCol < N.width && threadRow < N.height) {
        Ns[row + 2][col + 2] = N.elements[threadRow * N.width + threadCol];

        // padding up
        if(row == 0) {
            Ns[row][col + 2] = GetElement(N, threadRow - 2, threadCol);
            Ns[row + 1][col + 2] = GetElement(N, threadRow - 1, threadCol);
        }

        // padding left
        if(col == 0) {
            Ns[row + 2][col] = GetElement(N, threadRow, threadCol - 2);
            Ns[row + 2][col + 1] = GetElement(N, threadRow, threadCol - 1);

            if(row == 0) {
                Ns[row][col] = GetElement(N, threadRow - 2, threadCol - 2);
                Ns[row][col + 1] = GetElement(N, threadRow - 2, threadCol - 1);
                Ns[row + 1][col] = GetElement(N, threadRow - 1, threadCol - 2);
                Ns[row + 1][col + 1] = GetElement(N, threadRow - 1, threadCol - 1);
            }
            else if(row == BLOCK_SIZE - 1 || threadRow == N.height - 1) {
                Ns[row + 3][col] = GetElement(N, threadRow + 1, threadCol - 2);
                Ns[row + 3][col + 1] = GetElement(N, threadRow + 1, threadCol - 1);
                Ns[row + 4][col] = GetElement(N, threadRow + 2, threadCol - 2);
                Ns[row + 4][col + 1] = GetElement(N, threadRow + 2, threadCol - 1);
            }
        }

        //padding down
        if(row == BLOCK_SIZE - 1 || threadRow == N.height - 1) {
            Ns[row + 3][col + 2] = GetElement(N, threadRow + 1, threadCol);
            Ns[row + 4][col + 2] = GetElement(N, threadRow + 2, threadCol);
        }

        // padding right
        if(col == BLOCK_SIZE - 1 || threadCol == N.width - 1) {
            Ns[row + 2][col + 3] = GetElement(N, threadRow, threadCol + 1);
            Ns[row + 2][col + 4] = GetElement(N, threadRow, threadCol + 2);

            if(row == 0) {
                Ns[row][col + 3] = GetElement(N, threadRow - 2, threadCol + 1);
                Ns[row][col + 4] = GetElement(N, threadRow - 2, threadCol + 2);
                Ns[row + 1][col + 3] = GetElement(N, threadRow - 1, threadCol + 1);
                Ns[row + 1][col + 4] = GetElement(N, threadRow - 1, threadCol + 2);
            }
            else if(row == BLOCK_SIZE - 1 || threadRow == N.height - 1) {
                Ns[row + 3][col + 3] = GetElement(N, threadRow + 1, threadCol + 1);
                Ns[row + 3][col + 4] = GetElement(N, threadRow + 1, threadCol + 2);
                Ns[row + 4][col + 3] = GetElement(N, threadRow + 2, threadCol + 1);
                Ns[row + 4][col + 4] = GetElement(N, threadRow + 2, threadCol + 2);
            }
        }
    }

    // load kernel matrix into shared memory
    if(row == 0 && col == 0) {
        for(i = 0; i < KERNEL_SIZE; i++) {
            for(j = 0; j < KERNEL_SIZE; j++) {
                Ms[i][j] = M.elements[i * KERNEL_SIZE + j];
            }
        }
    }

    // Synchronize to make sure the sub-matrices are loaded
    // before starting the computation
    __syncthreads();

    
    if(threadRow < N.height && threadCol < N.width) {
        // perform computations
        for(i = 0; i < KERNEL_SIZE; i++) {
            for(j = 0; j < KERNEL_SIZE; j++) {
                int sharedRow = row + i;
                int sharedCol = col + j;

                Pvalue += Ms[i][j] * Ns[sharedRow][sharedCol];
            }
        }

        P.elements[threadRow * P.width + threadCol] = Pvalue;
    }
}

////////////////////////////////////////////////////////////////////////////////
// Returnează 1 dacă matricele sunt ~ egale
////////////////////////////////////////////////////////////////////////////////
int CompareMatrices(Matrix A, Matrix B)
{
    int i;
    if(A.width != B.width || A.height != B.height || A.pitch != B.pitch)
        return 0;
    int size = A.width * A.height;
    for(i = 0; i < size; i++)
        if(fabs(A.elements[i] - B.elements[i]) > MAX_ERR)
            return 0;
    return 1;
}
void GenerateRandomMatrix(Matrix m)
{
    int i;
    int size = m.width * m.height;

    srand(time(NULL));

    for(i = 0; i < size; i++)
        m.elements[i] = rand() / (float)RAND_MAX;
}

////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv) 
{
    int width = 0, height = 0;
    FILE *f, *out, *times;
    float tm;
    if(argc < 2)
    {
        printf("Argumente prea puține, trimiteți id-ul testului care trebuie rulat\n");
        return 0;
    }
    char name[100];
    sprintf(name, "./tests/test_%s.txt", argv[1]);

    if(atoi(argv[1]) == 1) {
        times = fopen("time_out.txt", "w");
    }
    else {
        times = fopen("time_out.txt", "a");
    }

    f = fopen(name, "r");
    out = fopen("out.txt", "a");
    fscanf(f, "%d%d", &width, &height);

    fprintf(times, "T%s 23 ", argv[1]);
    
    Matrix M;   //kernel de pe host
    Matrix N;   //matrice inițială de pe host
    Matrix P;   //rezultat fără memorie partajată calculat pe GPU
    Matrix PS;  //rezultatul cu memorie partajată calculat pe GPU
	
    M = AllocateMatrix(KERNEL_SIZE, KERNEL_SIZE);
    N = AllocateMatrix(width, height);		
    P = AllocateMatrix(width, height);
    PS = AllocateMatrix(width, height);

    GenerateRandomMatrix(M);
    GenerateRandomMatrix(N);
    
	// M * N pe device
    ConvolutionOnDevice(M, N, P, times);
    
    // M * N pe device cu memorie partajată
    ConvolutionOnDeviceShared(M, N, PS, times);

    // calculează rezultatul pe CPU pentru comparație
    Matrix reference = AllocateMatrix(P.height, P.width);
    
    //pentru măsurarea timpului de execuție în kernel
    StopWatchInterface *kernelTime = NULL;
    sdkCreateTimer(&kernelTime);
    sdkResetTimer(&kernelTime);

    sdkStartTimer(&kernelTime);
    computeGold(reference.elements, M.elements, N.elements, N.height, N.width);
    sdkStopTimer(&kernelTime);
    tm = sdkGetTimerValue(&kernelTime);
    // for plotting purposes
    // 2.5 is a very big time compared to CUDA times
    if(tm > 2.5) tm = 2.5;
    fprintf(times, "%f\n", tm);
        
    // verifică dacă rezultatul obținut pe device este cel așteptat
    int res = CompareMatrices(reference, P);
    printf("Test global %s\n", (1 == res) ? "PASSED" : "FAILED");
    fprintf(stderr, "Test global %s %s\n", argv[1], (1 == res) ? "PASSED" : "FAILED");

    // verifică dacă rezultatul obținut pe device cu memorie partajată este cel așteptat
    int ress = CompareMatrices(reference, PS);
    printf("Test shared %s\n", (1 == ress) ? "PASSED" : "FAILED");
    fprintf(stderr, "Test shared %s %s\n", argv[1], (1 == ress) ? "PASSED" : "FAILED");
   
	// Free matrices
    FreeMatrix(&M);
    FreeMatrix(&N);
    FreeMatrix(&P);
    FreeMatrix(&PS);

    fclose(f);
    fclose(out);
    fclose(times);
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
//! Run a simple test for CUDA
////////////////////////////////////////////////////////////////////////////////
void ConvolutionOnDevice(const Matrix M, const Matrix N, Matrix P, FILE *f)
{
    Matrix Md, Nd, Pd; //matricele corespunzătoare de pe device
    int sizeM = M.width * M.height;
    int sizeN = N.width * N.height;
    int sizeP = P.width * P.height;
    float tm;

    //pentru măsurarea timpului de execuție în kernel
    StopWatchInterface *kernelTime = NULL;
    sdkCreateTimer(&kernelTime);
    sdkResetTimer(&kernelTime);
    
    //alocare matrice de pe device
    Md = AllocateDeviceMatrix((int)M.width, (int)M.height);
    Nd = AllocateDeviceMatrix((int)N.width, (int)N.height);
    Pd = AllocateDeviceMatrix((int)P.width, (int)P.height);

    //copiere date de pe host (M, N) pe device (MD, Nd)
    cudaMemcpy(Md.elements, M.elements, sizeM * sizeof(float),
                cudaMemcpyHostToDevice);
    Md.width = Md.pitch = M.width;
    Md.height = M.height;

    cudaMemcpy(Nd.elements, N.elements, sizeN * sizeof(float),
                cudaMemcpyHostToDevice);
    Nd.width = Nd.pitch = N.width;
    Nd.height = N.height;

    //setare configurație de rulare a kernelului
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid((N.width + dimBlock.x - 1) / dimBlock.x,
                (N.height + dimBlock.y - 1) / dimBlock.y);

    sdkStartTimer(&kernelTime);

    //lansare în execuție kernelul
    ConvolutionKernel<<<dimGrid, dimBlock>>>(Md, Nd, Pd);

    cudaThreadSynchronize();
    sdkStopTimer(&kernelTime);
    tm = sdkGetTimerValue(&kernelTime);
    fprintf(f, "%f ", tm);
    printf ("Timp execuție kernel: %f ms\n", tm);

    //copiere rezultat in P
    cudaMemcpy(P.elements, Pd.elements, sizeP * sizeof(float),
                cudaMemcpyDeviceToHost);

    //eliberare memoria matricelor de pe device
    FreeDeviceMatrix(&Md);
    FreeDeviceMatrix(&Nd);
    FreeDeviceMatrix(&Pd);
}


void ConvolutionOnDeviceShared(const Matrix M, const Matrix N, Matrix P, FILE *f)
{
    Matrix Md, Nd, Pd; //matricele corespunzătoare de pe device
    int sizeM = M.width * M.height;
    int sizeN = N.width * N.height;
    int sizeP = P.width * P.height;
    float tm;

    //pentru măsurarea timpului de execuție în kernel
    StopWatchInterface *kernelTime = NULL;
    sdkCreateTimer(&kernelTime);
    sdkResetTimer(&kernelTime);

    //alocare matricele de pe device
    Md = AllocateDeviceMatrix((int)M.width, (int)M.height);
    Nd = AllocateDeviceMatrix((int)N.width, (int)N.height);
    Pd = AllocateDeviceMatrix((int)P.width, (int)P.height);

    //copiere date de pe host (M, N) pe device (MD, Nd)
    cudaMemcpy(Md.elements, M.elements, sizeM * sizeof(float), cudaMemcpyHostToDevice);
    Md.width = Md.pitch = M.width;
    Md.height = M.height;

    cudaMemcpy(Nd.elements, N.elements, sizeN * sizeof(float), cudaMemcpyHostToDevice);
    Nd.width = Nd.pitch = N.width;
    Nd.height = N.height;

    //setare configurație de rulare a kernelului
    dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE);
    dim3 dimGrid((N.width + dimBlock.x - 1) / dimBlock.x,
                (N.height + dimBlock.y - 1) / dimBlock.y);

    sdkStartTimer(&kernelTime);

    //lansare în execuție kernel
    ConvolutionKernelShared<<<dimGrid, dimBlock>>>(Md, Nd, Pd);

    cudaThreadSynchronize();
    sdkStopTimer(&kernelTime);
    tm = sdkGetTimerValue(&kernelTime);
    fprintf(f, "%f ", tm);
    printf ("Timp execuție kernel cu memorie partajată: %f ms\n", tm);

    //copiere rezultat in P
    cudaMemcpy(P.elements, Pd.elements, sizeP * sizeof(float),
                cudaMemcpyDeviceToHost);

    //eliberare memoria matricelor de pe device
    FreeDeviceMatrix(&Md);
    FreeDeviceMatrix(&Nd);
    FreeDeviceMatrix(&Pd);
}


// Alocă o matrice de dimensiune height*width pe device
Matrix AllocateDeviceMatrix(int width, int height)
{
    Matrix m;
    int size;

    m.width = m.pitch = width;
    m.height = height;
    size = width * height;

    cudaMalloc( (void**) &(m.elements), size *sizeof(float));

    return m;
}

// Alocă matrice pe host de dimensiune height*width
Matrix AllocateMatrix(int height, int width)
{
    Matrix M;
    M.width = M.pitch = width;
    M.height = height;
    int size = M.width * M.height;
	M.elements = (float*) malloc(size*sizeof(float));
    return M;
}	

// Eliberează o matrice de pe device
void FreeDeviceMatrix(Matrix* M)
{
    cudaFree(M->elements);
    M->elements = NULL;
}

// Eliberează o matrice de pe host
void FreeMatrix(Matrix* M)
{
    free(M->elements);
    M->elements = NULL;
}
