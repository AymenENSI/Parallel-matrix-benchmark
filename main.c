#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "matmul.h"
#include "utils.h"

/*
 Usage:
   ./matrix_mult N nthreads schedule chunk out_csv
   - N: matrix dimension (N x N)
   - nthreads: number of threads (1 for sequential baseline)
   - schedule: "static" or "dynamic"
   - chunk: iteration chunk size (int, >0). For static, chunk defines block-of-rows.
   - out_csv: path to CSV file to append results (use "-" to print only)
 Example:
   ./matrix_mult 512 8 static 16 results.csv
*/

int main(int argc, char **argv) {
    if (argc < 6) {
        fprintf(stderr,
            "Usage: %s N nthreads schedule(static|dynamic) chunk out_csv\n", argv[0]);
        return EXIT_FAILURE;
    }

    int N = atoi(argv[1]);
    int nthreads = atoi(argv[2]);
    char *sched_str = argv[3];
    int chunk = atoi(argv[4]);
    char *out_csv = argv[5];

    schedule_t schedule = SCHED_STATIC;
    if (strcmp(sched_str, "dynamic") == 0) schedule = SCHED_DYNAMIC;

    printf("Matrix mult: N=%d threads=%d schedule=%s chunk=%d\n",
           N, nthreads, sched_str, chunk);

    double *A = alloc_matrix(N);
    double *B = alloc_matrix(N);
    double *C = alloc_matrix(N);

    init_matrix_random(A, N, 1234);
    init_matrix_random(B, N, 4321);

    struct timespec t0, t1;

    double seq_time = 0.0;
    if (nthreads == 1) {
        zero_matrix(C, N);
        clock_gettime(CLOCK_MONOTONIC, &t0);
        matmul_seq(A, B, C, N);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        seq_time = elapsed_sec(t0, t1);
        printf("SEQ time: %f s\n", seq_time);
    } else {
        // compute seq baseline first to compute speedup
        double *Cseq = alloc_matrix(N);
        zero_matrix(Cseq, N);
        clock_gettime(CLOCK_MONOTONIC, &t0);
        matmul_seq(A, B, Cseq, N);
        clock_gettime(CLOCK_MONOTONIC, &t1);
        seq_time = elapsed_sec(t0, t1);
        printf("SEQ baseline time: %f s\n", seq_time);
        free_matrix(Cseq);
    }

    // Run parallel
    zero_matrix(C, N);
    clock_gettime(CLOCK_MONOTONIC, &t0);
    matmul_parallel(A, B, C, N, nthreads, schedule, chunk);
    clock_gettime(CLOCK_MONOTONIC, &t1);
    double par_time = elapsed_sec(t0, t1);
    printf("PAR time: %f s\n", par_time);

    double speedup = seq_time / par_time;
    double efficiency = speedup / (double)nthreads;

    printf("Speedup: %.4f  Efficiency: %.4f\n", speedup, efficiency);

    if (strcmp(out_csv, "-") != 0) {
        FILE *f = fopen(out_csv, "a");
        if (!f) {
            perror("fopen");
        } else {
            // CSV header should be created by run_tests.sh, we append rows here
            fprintf(f, "%d,%d,%s,%d,%.9f,%.9f,%.9f,%.9f\n",
                    N, nthreads, sched_str, chunk, seq_time, par_time, speedup, efficiency);
            fclose(f);
        }
    }

    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return EXIT_SUCCESS;
}
