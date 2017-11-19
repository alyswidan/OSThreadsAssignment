//
// Created by alyswidan on 19/11/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void *nonThreadedMatMult(void*);
void *ThreadedMatMultPerElement(void*);
void *ThreadMatMultPerRow(void*);
double** initMatrix(int,int);
double dot(double*, double*,int,int);
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
    nonThreadedMatMult(NULL);
    printMat(C,C_r,C_c,"C");

}
double** initMatrix(int r,int c){
    double **X = (double**)malloc(r*sizeof(double*));
    for (int i = 0; i < r; ++i) {
        X[i] = (double*)malloc(c * sizeof(double));
    }
    return X;
}

double dot(double *a, double *b,int sz_a,int sz_b){
    if(sz_a != sz_b)
    {
        fprintf(stderr,"incompatible sizes\n");
        exit(-1);
    }
    double result = 0;
    for (int i = 0; i < sz_a; ++i) {
        result += b[i] * a[i];
    }
    return result;
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
    if(A_c != B_r){
        fprintf(stderr,"incompatible matrix sizes\n");
        exit(0);

    }
    double ** B_T = transpose(B,B_r,B_c);
    for (int i = 0; i < A_r; ++i) {
        for (int j = 0; j < B_r; ++j) {
            C[i][j] = dot(A[i],B_T[i],A_r,B_r);
        }
    }
    return 0;
}



