//
// Created by alyswidan on 19/11/17.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <ctype.h>
#include "Multiply.h"
#define floatIsInt(f) (f-(int)f == 0)
#define randomMatrixMode 0
#define fileMatrixMode 1
#define currentMatrixMode fileMatrixMode
#define isVerbose 0  //print extra info during execution
#define N 10
const int maxMatElement = 50;
double **A,**B,**C;
int A_r = N,A_c=N,B_r=N,B_c=N,C_r,C_c;

int main(){

    /*names of files we are using*/
    char *Afile="./A-Matrix";
    char *Bfile="./B-Matrix";
    char *CfileNonThreaded="./C-Matrix";
    char *CfilePerElement="./C-MatrixPerElement";
    char *CfilePerRow="./C-MatrixPerRow";
    /*end names of files*/

    if(currentMatrixMode == fileMatrixMode){
        calculateDimensions(Afile,&A_r,&A_c);
        calculateDimensions(Bfile,&B_r,&B_c);
        if(isVerbose)
            printf("size A(%d,%d), size B(%d,%d)\n",A_r,A_c,B_r,B_c);
    }

    /*dimensions of the output matrix*/
    C_r = A_r, C_c = B_c;



    A = initMatrix(A_r, A_c);
    B = initMatrix(B_r, B_c);
    C = initMatrix(C_r, C_c);


    if(currentMatrixMode == randomMatrixMode){
        initRandomMat(A, A_r, A_c, maxMatElement);
        initRandomMat(B, B_r, B_c, maxMatElement);
    }
    else{
        ReadFromFile(Afile,A,A_r,A_c);
        ReadFromFile(Bfile,B,B_r,B_c);
    }
    if(isVerbose){
        printMat(A, A_r, A_c, "A");
        printMat(B, B_r, B_c, "B");
    }

    MatMultParams_t *params = (MatMultParams_t *) malloc(sizeof(MatMultParams_t));
    MatMultParams_t *params_nt = (MatMultParams_t *) malloc(sizeof(MatMultParams_t));
    double **B_T = transpose(B, B_r,B_c);
    initMatMultParams(params,A,B,C,A_r,A_c,B_r,B_c);
    initMatMultParams(params_nt,A,B_T,C,A_r,A_c,B_c,B_r);

    printf("non-threaded = %lf\n",benchmark(nonThreadedMatMult,params_nt));
    if(currentMatrixMode == fileMatrixMode)
        WriteFromFile(CfileNonThreaded,C,C_r,C_c);
    printf("threaded by row = %lf\n",benchmark(ThreadMatMultPerRow,params));
    if(currentMatrixMode == fileMatrixMode)
        WriteFromFile(CfilePerRow,C,C_r,C_c);
    printf("threaded by element= %lf\n",benchmark(ThreadedMatMultPerElement,params));
    if(currentMatrixMode == fileMatrixMode)
        WriteFromFile(CfilePerElement,C,C_r,C_c);

}

double **initMatrix(int r, int c) {
    double **X = (double **) malloc(r * sizeof(double *));
    for (int i = 0; i < r; ++i) {
        X[i] = (double*)malloc(c * sizeof(double));
    }
    return X;
}

void calculateDimensions(char *fileName,int *rows,int *cols){
    FILE *filePtr = fopen(fileName,"r");
    *cols = 0;
    int current,prev='a',element_cnt=0;
    //count the number of spaces in the first line
    //ignore a space if the previous character was a space
    while((current=getc(filePtr)) != '\n') {
        if(isspace(current) && !isspace(prev))
            (*cols)++;
        prev = current;
    }
    (*cols)++;
    element_cnt = *cols; // add one to the number of spaces

    //count the rest of the elements
    double current_element;
    while(fscanf(filePtr,"%lf",&current_element)!=EOF){
        element_cnt++;
    }
    //if dividing number of elements by cols isn't a whole number of the rows has less columns
    if(!floatIsInt(1.0*element_cnt / *cols)){
        fprintf(stderr,"number of elements per row aren't equal\n");
        abort();
    }
    *rows = element_cnt / *cols;

}
void ReadFromFile(char* s,double** matrix,int n, int m){
    FILE *fp;
    fp = fopen(s, "r");

    for(int i=0;i< n;i++){
        for(int j=0 ;j<m;j++){

            fscanf(fp, "%lf ",&matrix[i][j]);

        }
    }
    fclose(fp);

}
void WriteFromFile(char* s,double** matrix,int n, int m){

    FILE *fp;
    fp = fopen(s, "w+");
    fprintf(fp, " ");
    for(int i=0;i< n;i++){
        for(int j=0 ;j<m;j++){

            fprintf(fp, "%lf ",matrix[i][j]);

        }
        fprintf(fp, "\n ");
    }
    fclose(fp);

}
void *dot(void *params) {
    DotParams_t *data = (DotParams_t *) params;
    if (data->sz_a != data->sz_b) {
        fprintf(stderr, "incompatible sizes\n");
        exit(-1);
    }
    double result = 0;
    for (int i = 0; i < data->sz_a; ++i) {
        result += data->b[i] * data->a[i];
    }
    *data->result = result;
//    free(data);
    return 0;
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

void printVector(double *a, int size) {
    for (int i = 0; i < size; i++) {
        printf("%lf ", a[i]);
    }
    printf("\n");
}

void *nonThreadedMatMult(void *param) { /*A * B */

    MatMultParams_t *data = (MatMultParams_t *) param;
    if (data->A_c != data->B_c) {
        fprintf(stderr, "incompatible matrix sizes\n");
        printf("\nAC%d   BR%d\n",data->A_c,data->B_c);
        exit(0);
    }
    DotParams_t *dotParams = (DotParams_t *) malloc(sizeof(DotParams_t));

    for (int i = 0; i < data->A_r; ++i) {
        for (int j = 0; j < data->B_r; ++j) {
            initDotParams(dotParams,data->A[i],data->B[j],data->A_c,data->B_c,&(data->C[i][j]));
            dot((void *) dotParams);

        }
    }
//  free(dotParams);
   free(param);
    return 0;

}

void *ThreadMatMultPerRow(void *param) {

    MatMultParams_t *data = (MatMultParams_t *) param;
    pthread_t *threads = (pthread_t *) malloc(data->A_r * sizeof(pthread_t));
    MatMultParams_t *newdata1 = (MatMultParams_t *) malloc(sizeof(MatMultParams_t));
    memcpy(newdata1, data, sizeof(MatMultParams_t));
    double **B_T = transpose(data->B, data->B_r, data->B_c);
    int B_T_r = data->B_c,B_T_c = data->B_r;
    printf("btr  btc %d %d", B_T_r,B_T_c);
    for (int i = 0; i < data->A_r; i++) {
        double **newA = initMatrix(1, data->A_c);
        double **newC = initMatrix(1, B_T_r);
        MatMultParams_t *newdata = (MatMultParams_t *) malloc(sizeof(MatMultParams_t));
        initMatMultParams(newdata,newA,B_T,newC,1,newdata1->A_c,B_T_r,B_T_c);
        pthread_create(&threads[i], NULL, nonThreadedMatMult, newdata);

    }

    for (int i = 0; i < data->A_r; i++) {
        pthread_join(threads[i], NULL);

    }

    free(newdata1);
    free(threads);
    return 0;
}

void *ThreadedMatMultPerElement(void *params) {

    MatMultParams_t *data = (MatMultParams_t *) params;
    if (data->A_c != data->B_r) {
        fprintf(stderr, "incompatible matrix sizes  %d   %d\n   ",data->A_c,data->B_r);
        exit(0);
    }
    int C_r = data->A_r, C_c = data->B_c;


    double **B_T = transpose(data->B, data->B_r, data->B_c);

    pthread_t *threads = (pthread_t *) malloc(C_r * C_c * sizeof(pthread_t));

    for (int i = 0; i < data->A_r; ++i) {
        for (int j = 0; j < data->B_c; ++j) {
            DotParams_t *dotParams = (DotParams_t *) malloc(sizeof(struct DotParams));
            initDotParams(dotParams,data->A[i],B_T[j],data->A_c,data->B_r,&(data->C[i][j]));
            pthread_create(&threads[j + data->B_c * i], NULL, dot, dotParams);
        }
    }
    for (int i = 0; i < C_r * C_c; ++i) {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    return 0;
}

double get_time() {
    struct timeval t;
    struct timezone tzp;
    gettimeofday(&t, &tzp);
    return t.tv_sec + t.tv_usec * 1e-6;
}

double benchmark(matmult fun,void*params){
    double start = get_time();
    fun(params);
    double end = get_time();
    return end-start;
}

void initDotParams(DotParams_t*params,double* a,double* b,
                   int sz_a,int sz_b,
                   double *result){
    params->a = a;
    params->b = b;
    params->sz_a = sz_a;
    params->sz_b = sz_b;
    params->result = result;
}
void initMatMultParams(MatMultParams_t*params,
                       double** A, double** B, double** C,
                       int A_r,int A_c,int B_r,int B_c){

    params->A = A;
    params->B = B;
    params->C = C;
    params->A_r = A_r;
    params->A_c = A_c;
    params->B_r = B_r;
    params->B_c = B_c;
}




