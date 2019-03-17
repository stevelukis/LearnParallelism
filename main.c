#include <omp.h>
#include <stdio.h>

#define NUM_OF_THREADS 4

static long num_steps = 100000000;

double dur(double start, double finish) {
    return finish - start;
}

int main() {
    double x, step, sum, sPi, pPi, oPi;
    step = 1.0 / (double) num_steps;

    double sStart, sFinish, pStart, pFinish, oStart, oFinish;

    //serial
    sStart = omp_get_wtime();
    for (long i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        sum = sum + 4.0 / (1.0 + x * x);
    }
    sPi = step * sum;
    sFinish = omp_get_wtime();


    //Parallel
    sum = 0;
    omp_set_num_threads(NUM_OF_THREADS);
    pStart = omp_get_wtime();
#pragma omp parallel
    {
        double x;
#pragma omp for reduction(+:sum)
        for (long i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }
    }
    pPi = step * sum;
    pFinish = omp_get_wtime();

    //P(1)
    sum = 0;
    omp_set_num_threads(1);
    oStart = omp_get_wtime();
#pragma omp parallel
    {
        double x;
#pragma omp for reduction(+:sum)
        for (long i = 0; i < num_steps; i++) {
            x = (i + 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }
    }
    pPi = step * sum;
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