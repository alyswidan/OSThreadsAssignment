//
// Created by alyswidan on 21/11/17.
//

#pragma once
typedef struct MatMultParams{
    double** A;
    double** B;
    double** C;
    int A_r,A_c,B_r,B_c;
}MatMultParams_t;

typedef struct DotParams{
    double* a;
    double* b;
    int sz_a,sz_b;
    double *result;
}DotParams_t;

typedef void *matmult(void*);
void calculateDimensions(char *fileName,int *rows,int *cols);
void initDotParams(DotParams_t*,double* a,double* b,int sz_a,int sz_b,double *result);
void initMatMultParams(MatMultParams_t*,double** A, double** B, double** C, int A_r,int A_c,int B_r,int B_c);
double benchmark(matmult,void*);
double get_time();
void *nonThreadedMatMult(void*);
void *ThreadedMatMultPerElement(void*);
void *ThreadMatMultPerRow(void*);
double** initMatrix(int,int);
void* dot(void *params);
double **transpose(double**, int,int);
void initRandomMat(double**,int,int,int);
void printMat(double**,int,int,char*);
void  printVector(double*,int);
