#include <omp.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

#define NUM_OF_THREADS 4
#define ARRAY_SIZE 100000

int* arrayA;
int* arrayC;

double dur(double start, double finish) {
    return finish - start;
}

int *init(int size) {
    int *arr = malloc(sizeof(int) * size);
    return arr;
}

int *randoms(int size) {
    int *arr = init(size);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;
    }
    return arr;
}

void loop3(int start, int end) {
    int numOfItem = end - start;
    if (numOfItem > 10) {
        numOfItem /= 2;
        loop3(start, start + numOfItem);
        loop3(start + numOfItem, end);
        return;
    }

    for (int i = start; i < end; i++) {
        arrayC[i] = 100 * arrayA[i];
    }
}

void loop2(int start, int end) {
    int numOfItem = end - start;
    if (numOfItem > 10) {
        numOfItem /= 2;
#pragma omp task
        loop3(start, start + numOfItem);
#pragma omp task
        loop3(start + numOfItem, end);
#pragma omp taskwait
        return;
    }

    for (int i = start; i < end; i++) {
        arrayC[i] = 100 * arrayA[i];
    }
}

void loop(int start, int end) {
    int numOfItem = end - start;
    if (numOfItem > 10) {
        numOfItem /= 2;
#pragma omp task
        loop2(start, start + numOfItem);
#pragma omp task
        loop2(start + numOfItem, end);
#pragma omp taskwait
        return;
    }

    for (int i = start; i < end; i++) {
        arrayC[i] = 100 * arrayA[i];
    }
}

int main() {
    arrayA = randoms(ARRAY_SIZE);
    arrayC = init(ARRAY_SIZE);

    double sStart, sFinish, pStart, pFinish, oStart, oFinish;

    sStart = omp_get_wtime();
    loop(0, ARRAY_SIZE);
    sFinish = omp_get_wtime();

    pStart = omp_get_wtime();
#pragma omp parallel num_threads(NUM_OF_THREADS)
    {
#pragma omp single
        loop(0, ARRAY_SIZE);
    };
    pFinish = omp_get_wtime();

    oStart = omp_get_wtime();
#pragma omp parallel num_threads(1)
    {
#pragma omp single
        loop(0, ARRAY_SIZE);
    };
    oFinish = omp_get_wtime();

    double serialTime = dur(sStart, sFinish);
    double parallelTime = dur(pStart, pFinish);
    double parallelOneTime = dur(oStart, oFinish);
    double speedUp = serialTime / parallelTime;
    double efficiency = speedUp / NUM_OF_THREADS * 100;


    printf("Serial time = %fs\n", serialTime);
    printf("Parallel time = %fs\n", parallelTime);
    printf("Paralel(1) time = %f\n\n", parallelOneTime);

    printf("Overhead = %fs\n", parallelOneTime - serialTime);
    printf("Speedup = %fx\n", speedUp);
    printf("Efficiency = %f%\n", efficiency);
}