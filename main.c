#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

#define NUM_OF_THREADS 2

int arraySize = 1000;

int *arrA;
int *arrB;
int *arrC;

int* init(int size) {
    int *arr = malloc(sizeof(int) * size * size);
    return arr;
}

int* randoms(int size) {
    int *arr = init(size);
    for (int i = 0; i < size * size; i++) {
        arr[i] = rand() % 100;
    }
    return arr;
}

int* zeros(int size) {
    int *arr = init(size);
    for (int i = 0; i < size * size; i++) {
        arr[i] = rand() % 100;
    }
    return arr;
}

double dur(clock_t start, clock_t finish) {
    return ((double) (finish - start) / CLOCKS_PER_SEC);
}

double dur_linux(double start, double finish) {
    return finish - start;
}

int main() {
    arrA = randoms(arraySize);
    arrB = randoms(arraySize);
    arrC = zeros(arraySize);

//    clock_t sStart, sFinish, pStart, pFinish;
    double sStart, sFinish, pStart, pFinish;

    //transpose array B
    int* arrB_transpose = zeros(arraySize);
#pragma omp for
    for (int i = 0; i < arraySize; i++)
        for(int j = 0 ; j < arraySize; j++)
            arrB_transpose[arraySize * i + j] = arrB[arraySize * j + i];

    //serial
//    sStart = clock();
    sStart = omp_get_wtime();
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            int sum = 0;
            for (int k = 0; k < arraySize; k++) {
                sum += arrA[arraySize * i + k] * arrB[arraySize * j + k];
            }
            arrC[arraySize * i + j] = sum;
        }
    }
//    sFinish = clock();
    sFinish = omp_get_wtime();

    //parallel
    omp_set_num_threads(NUM_OF_THREADS);

//    pStart = clock();
    pStart = omp_get_wtime();

#pragma omp parallel for
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            int sum = 0;
            for (int k = 0; k < arraySize; k++) {
                sum += arrA[arraySize * i + k] * arrB[arraySize * j + k];
            }
            arrC[arraySize * i + j] = sum;
        }
    }
//    pFinish = clock();
    pFinish = omp_get_wtime();

//    printf("Serial time = %f\n", dur(sStart, sFinish));
//    printf("Paralel time = %f\n", dur(pStart, pFinish));

    printf("Serial time = %fs\n", dur_linux(sStart, sFinish));
    printf("Paralel time = %fs\n", dur_linux(pStart, pFinish));

    return 0;
}

