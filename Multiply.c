//
// Created by alyswidan on 19/11/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

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
    double result;
}DotParams_t;

void *nonThreadedMatMult(void*);
void *ThreadedMatMultPerElement(void*);
void *ThreadMatMultPerRow(void*);
double** initMatrix(int,int);
void* dot(void *params);
double **transpose(double**, int,int);
void initRandomMat(double**,int,int,int);
void printMat(double**,int,int,char*);
void  printVector(double*,int);
double **A,**B,**C;
int A_r = 5,A_c=5,B_r=5,B_c=5,C_r,C_c;
const int limit = 50;
int main(){

    C_r = A_r,C_c = B_c;
    A = initMatrix(A_r,A_c);
    B = initMatrix(B_r,B_c);
    C = initMatrix(C_r,C_c);
    initRandomMat(A,A_r,A_c,limit);
    initRandomMat(B,B_r,B_c,limit);
    printMat(A,A_r,A_c,"A");
    printMat(B,B_r,B_c,"B");
    MatMultParams_t* params = malloc(sizeof(MatMultParams_t));
    params->A = A;
    params->B = B;
    params->C = C;
    params->A_r = A_r;
    params->B_r =B_r;
    params->A_c = A_c;
    params->B_c = B_c;
    nonThreadedMatMult(params);
    printMat(C,C_r,C_c,"C");

}
double** initMatrix(int r,int c){
    double **X = (double**)malloc(r*sizeof(double*));
    for (int i = 0; i < r; ++i) {
        X[i] = (double*)malloc(c * sizeof(double));
    }
    return X;
}

void* dot(void *params){
    DotParams_t *data = (DotParams_t*)params;
    if(data->sz_a != data->sz_b)
    {
        fprintf(stderr,"incompatible sizes\n");
        exit(-1);
    }
    double result = 0;
    for (int i = 0; i < data->sz_a; ++i) {
        result += data->b[i] * data->a[i];
    }
    data->result = result;
}

double **transpose(double **X, int X_r,int X_c){
    double **T = initMatrix(X_c,X_r);

    for (int i = 0; i < X_r; ++i) {
        for (int j = 0; j < X_c; ++j) {
            T[j][i] = X[i][j];
        }
    }
    return T;
}
void initRandomMat(double** X,int r,int c,int limit){
    time_t seed;
    srand((unsigned) time(&seed));
    for (int i = 0; i < r;++i) {
        for (int j = 0; j < c; ++j) {
            X[i][j] = rand() % limit;
        }
    }
}

void printMat(double** X,int r,int c,char* name){
    printf("%s = \n",name);
    for (int i = 0; i < r; ++i) {
        for (int j = 0; j < c; ++j) {
            printf("%lf ",X[i][j]);
        }
        printf("\n");
    }
}

void  printVector(double* a,int size){
    for(int i =0;i<size;i++){

        printf("%lf ",a[i]);

    }
    printf("\n");
}
void *nonThreadedMatMult(void* param){ /*A * B */
    MatMultParams_t* data = (MatMultParams_t*)param;
    if(data->A_c != data->B_r){
        fprintf(stderr,"incompatible matrix sizes\n");
        exit(0);
    }
    DotParams_t* dotParams = (DotParams_t*)malloc(sizeof(struct DotParams));
    double ** B_T = transpose(data->B,data->B_r,data->B_c);
    for (int i = 0; i < A_r; ++i) {
        for (int j = 0; j < B_c; ++j) {
            dotParams->a = A[i];
            dotParams->b = B_T[j];
            dotParams->sz_a = A_c;
            dotParams->sz_b = B_r;
            dot((void*)dotParams);
            C[i][j] = dotParams->result;
        }
    }
    return 0;
}



