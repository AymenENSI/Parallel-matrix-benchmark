#ifndef UTILS_H
#define UTILS_H

#include <time.h>

double *alloc_matrix(int N);
void free_matrix(double *M);
void init_matrix_random(double *M, int N, unsigned int seed);
void zero_matrix(double *M, int N);
double elapsed_sec(struct timespec start, struct timespec end);

#endif // UTILS_H
