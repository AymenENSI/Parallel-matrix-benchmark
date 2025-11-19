#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/* allocate a contiguous N*N matrix (row-major) */
double *alloc_matrix(int N) {
    double *M = (double*) malloc(sizeof(double) * N * N);
    if (!M) {
        fprintf(stderr, "alloc_matrix: allocation failed for N=%d\n", N);
        exit(EXIT_FAILURE);
    }
    return M;
}

void free_matrix(double *M) {
    free(M);
}

void init_matrix_random(double *M, int N, unsigned int seed) {
    srand(seed);
    for (long i = 0; i < (long)N * N; ++i) {
        // small-ish values to avoid overflow
        M[i] = ((double)rand() / (double)RAND_MAX) * 2.0 - 1.0;
    }
}

void zero_matrix(double *M, int N) {
    memset(M, 0, sizeof(double) * N * N);
}   

double elapsed_sec(struct timespec start, struct timespec end) {
    double s = (double)(end.tv_sec - start.tv_sec);
    double ns = (double)(end.tv_nsec - start.tv_nsec) * 1e-9;
    return s + ns;
}
