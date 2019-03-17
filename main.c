#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

#define NUM_OF_THREADS 4

int arraySize = 1000;

int *arrA;
int *arrB;
int *arrC;

int *init(int size) {
    int *arr = malloc(sizeof(int) * size * size);
    return arr;
}

int *randoms(int size) {
    int *arr = init(size);
    for (int i = 0; i < size * size; i++) {
        arr[i] = rand() % 100;
    }
    return arr;
}

int *zeros(int size) {
    int *arr = init(size);
    for (int i = 0; i < size * size; i++) {
        arr[i] = rand() % 100;
    }
    return arr;
}

double dur(clock_t start, clock_t finish) {
    return ((double) (finish - start) / CLOCKS_PER_SEC);
}

int main() {
    arrA = randoms(arraySize);
    arrB = randoms(arraySize);
    arrC = zeros(arraySize);

    clock_t sStart, sFinish, pStart, pFinish;

    //serial
    sStart = clock();
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            int sum = 0;
            for (int k = 0; k < arraySize; k++) {
                sum += arrA[arraySize * i + k] * arrB[arraySize * k + j];
            }
            arrC[arraySize * i + j] = sum;
        }
    }
    sFinish = clock();

    //paralel
    omp_set_num_threads(NUM_OF_THREADS);
    pStart = clock();

#pragma omp parallel for
    for (int i = 0; i < arraySize; i++) {
        for (int j = 0; j < arraySize; j++) {
            int sum = 0;
            for (int k = 0; k < arraySize; k++) {
                sum += arrA[arraySize * i + k] * arrB[arraySize * k + j];
            }
            arrC[arraySize * i + j] = sum;
        }
    }
    pFinish = clock();

    printf("Serial time = %f\n", dur(sStart, sFinish));
    printf("Paralel time = %f\n", dur(pStart, pFinish));

    return 0;
}

