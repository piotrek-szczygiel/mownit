#include <gsl/gsl_blas.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static struct timespec start, stop;
static long long unsigned int difference;

static FILE* f;

void start_clock()
{
    clock_gettime(CLOCK_REALTIME, &start);
}

void stop_clock()
{
    clock_gettime(CLOCK_REALTIME, &stop);
    difference = (stop.tv_sec * 1000000000 + stop.tv_nsec)
        - (start.tv_sec * 1000000000 + start.tv_nsec);
}

void blas(unsigned int size)
{
    gsl_matrix* m1 = gsl_matrix_alloc(size, size);
    gsl_matrix* m2 = gsl_matrix_alloc(size, size);
    gsl_matrix* m3 = gsl_matrix_alloc(size, size);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            gsl_matrix_set(m1, i, j, rand());
            gsl_matrix_set(m2, i, j, rand());
        }
    }

    start_clock();
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, m1, m2, 1.0, m3);
    stop_clock();

    gsl_matrix_free(m1);
    gsl_matrix_free(m2);
}

void measure(unsigned int size)
{
    blas(size);
    unsigned long long blas_time_ns = difference;

    fprintf(f, "%d,%lld\n", size, blas_time_ns);
}

int main()
{
    srand(time(NULL));
    blas(100);

    f = fopen("results.csv", "w");
    fprintf(f, "size,blas\n");

    for (int size = 10; size <= 100; size += 1) {
        for (int i = 0; i < 10; i++) {
            measure(size);
        }
    }

    fclose(f);
    return 0;
}
