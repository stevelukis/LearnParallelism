#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<time.h>

#define NUM_OF_THREADS 4

#define FIBONACCI_NUMBER 20


long fib_3(int n) {
    long x, y;

    if (n < 2) return n;
    else {
//        printf("Number of threads : %d\n", omp_get_thread_num());
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
            printf("Number of threads : %d\n", omp_get_thread_num());
            x = fib_3(n - 1);
        }
#pragma omp task shared(y)
        {
            printf("Number of threads : %d\n", omp_get_thread_num());
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
            printf("Number of threads : %d\n", omp_get_thread_num());
            x = fib_2(n - 1);
        }
        #pragma omp task shared(y)
        {
            printf("Number of threads : %d\n", omp_get_thread_num());
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

    sStart = omp_get_wtime();
    long fibSerial = fib_3(FIBONACCI_NUMBER);
    sFinish = omp_get_wtime();

    pStart = omp_get_wtime();
    long fibParallel;
#pragma omp parallel
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

