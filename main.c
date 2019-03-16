#include<stdio.h>
#include<omp.h>

#define NUM_OF_THREADS 4

#define FIBONACCI_NUMBER 40


long fib_3(int n) {
    long x, y;

    if (n < 2) return n;
    else {
        x = fib_3(n - 1);
        y = fib_3(n - 2);
        return x + y;
    }
}

long fib_2(int n) {
    long x, y;

    if (n < 2) return n;
    else {
#pragma omp task shared(x)
        {
            x = fib_3(n - 1);
        }
#pragma omp task shared(y)
        {
            y = fib_3(n - 2);
        }
#pragma omp taskwait
        return x + y;
    }
}

long fib_1(int n) {
    long x, y;

    if (n < 2) return n;
    else {
        #pragma omp task shared(x)
        {
            x = fib_2(n - 1);
        }
        #pragma omp task shared(y)
        {
            y = fib_2(n - 2);
        }
        #pragma omp taskwait
        return x + y;
    }
}

double dur_linux(double start, double finish) {
    return finish - start;
}

int main() {
    double sStart, sFinish, pStart, pFinish;

    omp_set_num_threads(NUM_OF_THREADS);

    printf("Serial in progress...");

    sStart = omp_get_wtime();
    long fibSerial = fib_3(FIBONACCI_NUMBER);
    sFinish = omp_get_wtime();

    printf("Parallel in progress...");

    pStart = omp_get_wtime();
    long fibParallel;
#pragma omp parallel num_threads(NUM_OF_THREADS)
    {
#pragma omp single
        fibParallel = fib_1(FIBONACCI_NUMBER);
    };
    pFinish = omp_get_wtime();

    printf("Serial result = %ld\n", fibSerial);
    printf("Parallel result = %ld\n\n", fibParallel);


    printf("Serial time = %fs\n", dur_linux(sStart, sFinish));
    printf("Parallel time = %fs\n", dur_linux(pStart, pFinish));

    return 0;
}

