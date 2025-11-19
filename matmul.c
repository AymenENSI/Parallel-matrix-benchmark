#include "matmul.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define IDX(i,j,N) ((i)*(N) + (j))

/* Sequential matrix multiplication: C = A * B */
void matmul_seq(double *A, double *B, double *C, int N) {
    // assume C is zeroed or overwritten
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            double sum = 0.0;
            for (int k = 0; k < N; ++k) {
                sum += A[IDX(i,k,N)] * B[IDX(k,j,N)];
            }
            C[IDX(i,j,N)] = sum;
        }
    }
}

static void *worker(void *argp) {
    thread_arg_t *arg = (thread_arg_t*) argp;
    int N = arg->N;
    int chunk = arg->chunk > 0 ? arg->chunk : 1;

    if (arg->schedule == SCHED_STATIC) {
        int start = arg->tid * chunk;
        for (int row = start; row < N; row += chunk * arg->nthreads) {
            int end = row + chunk;
            if (end > N) end = N;
            for (int i = row; i < end; ++i) {
                for (int j = 0; j < N; ++j) {
                    double sum = 0.0;
                    for (int k = 0; k < N; ++k) {
                        sum += arg->A[IDX(i,k,N)] * arg->B[IDX(k,j,N)];
                    }
                    arg->C[IDX(i,j,N)] = sum;
                }
            }
        }
    } else {
        // dynamic schedule: threads pull next_row with mutex
        while (1) {
            pthread_mutex_lock(arg->lock);
            int row = *(arg->next_row);
            *(arg->next_row) += chunk;
            pthread_mutex_unlock(arg->lock);

            if (row >= N) break;

            int end = row + chunk;
            if (end > N) end = N;
            for (int i = row; i < end; ++i) {
                for (int j = 0; j < N; ++j) {
                    double sum = 0.0;
                    for (int k = 0; k < N; ++k) {
                        sum += arg->A[IDX(i,k,N)] * arg->B[IDX(k,j,N)];
                    }
                    arg->C[IDX(i,j,N)] = sum;
                }
            }
        }
    }

    return NULL;
}

/* Parallel matrix multiplication entry */
void matmul_parallel(double *A, double *B, double *C, int N,
                     int nthreads, schedule_t schedule, int chunk) {

    pthread_t *threads = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    thread_arg_t *args = (thread_arg_t*) malloc(sizeof(thread_arg_t) * nthreads);

    int next_row = 0;
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    for (int t = 0; t < nthreads; ++t) {
        args[t].tid = t;
        args[t].nthreads = nthreads;
        args[t].N = N;
        args[t].chunk = chunk > 0 ? chunk : 1;
        args[t].schedule = schedule;
        args[t].A = A;
        args[t].B = B;
        args[t].C = C;
        args[t].next_row = &next_row;
        args[t].lock = &lock;
        pthread_create(&threads[t], NULL, worker, &args[t]);
    }

    for (int t = 0; t < nthreads; ++t) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&lock);
    free(threads);
    free(args);
}
