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

void naive(unsigned int size)
{
    int** m1 = malloc(size * sizeof(int*));
    int** m2 = malloc(size * sizeof(int*));
    int** m3 = malloc(size * sizeof(int*));
    for (int i = 0; i < size; ++i) {
        m1[i] = calloc(size, sizeof(int));
        m2[i] = calloc(size, sizeof(int));
        m3[i] = calloc(size, sizeof(int));
    }

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            m1[i][j] = rand();
            m2[i][j] = rand();
        }
    }

    start_clock();
    for (int j = 0; j < size; ++j) {
        for (int k = 0; k < size; ++k) {
            for (int i = 0; i < size; ++i) {
                m3[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    stop_clock();

    free(m1);
    free(m2);
    free(m3);
}

void better(unsigned int size)
{
    int** m1 = malloc(size * sizeof(int*));
    int** m2 = malloc(size * sizeof(int*));
    int** m3 = malloc(size * sizeof(int*));
    for (int i = 0; i < size; ++i) {
        m1[i] = calloc(size, sizeof(int));
        m2[i] = calloc(size, sizeof(int));
        m3[i] = calloc(size, sizeof(int));
    }

    start_clock();
    for (int i = 0; i < size; ++i) {
        for (int k = 0; k < size; ++k) {
            for (int j = 0; j < size; ++j) {
                m3[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    stop_clock();

    free(m1);
    free(m2);
    free(m3);
}

void measure(unsigned int size)
{

    naive(size);
    unsigned long long naive_time_ns = difference;

    better(size);
    unsigned long long better_time_ns = difference;

    blas(size);
    unsigned long long blas_time_ns = difference;

    fprintf(f, "%d,%lld,%lld,%lld\n", size, naive_time_ns, better_time_ns, blas_time_ns);
}

int main()
{
    srand(time(NULL));
    naive(10);
    better(10);
    blas(10);

    f = fopen("results.csv", "w");
    fprintf(f, "size,naive,better,blas\n");

    for (int size = 10; size <= 200; ++size) {
        for (int i = 0; i < 10; ++i) {
            measure(size);
        }
    }

    fclose(f);
    return 0;
}
