#include <stdio.h>
#include <omp.h>
#include <time.h>

#define NUM_THREADS 4

static long long num_steps = 1000;
double step;

double dur(double start, double finish);

int main() {
    step = 1.0 / (double) num_steps;

    omp_set_num_threads(NUM_THREADS);

    double pStart, pFinish, sStart, sFinish;

    double parallelPi = 0;
    double serialPi = 0;
    double x = 0;

    pStart = omp_get_wtime();
#pragma omp parallel for private(x) reduction(+:parallelPi)
    for (long long i = 0; i < num_steps; i++){
        x = (i + 0.5) * step;
        parallelPi += 4.0 / (1.0 + x * x);
    }

    parallelPi *= step;

    pFinish = omp_get_wtime();


    sStart = omp_get_wtime();
    for (int i = 0; i < num_steps; i++) {
        x = (i + 0.5) * step;
        serialPi += 4.0/(1.0 + x * x);
    }

    serialPi *= step;

    sFinish = omp_get_wtime();

    printf("Parallel PI = %.10f\nTime = %.10fs\n\n", parallelPi, dur(pStart, pFinish));
    printf("Serial PI = %.10f\nTime = %.10fs\n", serialPi, dur(sStart, sFinish));

}

double dur(double start, double finish) {
    return ((finish-start));
}