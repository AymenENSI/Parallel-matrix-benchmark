#ifndef MATMUL_H
#define MATMUL_H

#include <pthread.h>

typedef enum { SCHED_STATIC = 0, SCHED_DYNAMIC = 1 } schedule_t;

typedef struct {
    int tid;
    int nthreads;
    int N;
    int chunk;
    schedule_t schedule;
    double *A;
    double *B;
    double *C;
    int *next_row;
    pthread_mutex_t *lock;
} thread_arg_t;

/* API */
void matmul_seq(double *A, double *B, double *C, int N);
void matmul_parallel(double *A, double *B, double *C, int N,
                     int nthreads, schedule_t schedule, int chunk);

#endif // MATMUL_H
