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

void vectors(unsigned int size)
{
    gsl_vector* v1 = gsl_vector_alloc(size);
    gsl_vector* v2 = gsl_vector_alloc(size);

    for (int i = 0; i < size; ++i) {
        gsl_vector_set(v1, i, rand());
        gsl_vector_set(v2, i, rand());
    }

    double result;

    start_clock();
    gsl_blas_ddot(v1, v2, &result);
    stop_clock();

    gsl_vector_free(v1);
    gsl_vector_free(v2);
}

void matrix_vector(unsigned int size)
{
    gsl_vector* v = gsl_vector_alloc(size);
    gsl_matrix* m = gsl_matrix_alloc(size, size);

    for (int i = 0; i < size; ++i)
        gsl_vector_set(v, i, rand());

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            gsl_matrix_set(m, i, j, rand());

    gsl_vector* r = gsl_vector_alloc(size);

    start_clock();
    gsl_blas_dgemv(CblasNoTrans, 1, m, v, 1, r);
    stop_clock();

    gsl_vector_free(v);
    gsl_matrix_free(m);
}

void measure(unsigned int size)
{
    vectors(size);
    unsigned long long vv_time_ns = difference;

    matrix_vector(size);
    unsigned long long mv_time_ns = difference;

    fprintf(f, "%d,%lld,%lld\n", size, vv_time_ns, mv_time_ns);
}

int main()
{
    srand(time(NULL));
    vectors(100);
    matrix_vector(100);

    f = fopen("results.csv", "w");
    fprintf(f, "size,vector_ns,matrix_ns\n");

    for (int size = 10; size <= 100; size += 1) {
        for (int i = 0; i < 10; i++) {
            measure(size);
        }
    }

    fclose(f);
    return 0;
}
