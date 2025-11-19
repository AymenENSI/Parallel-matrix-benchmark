#!/bin/bash
# Script d'exécution automatique.
# Attention: les grandes tailles prennent beaucoup de temps (surtout séquentiel).
set -e

EXE=./matrix_mult
OUT=results.csv

# header
echo "N,threads,schedule,chunk,seq_time,par_time,speedup,efficiency" > $OUT

SIZES=(128 256 512 1024 2048)   # adapte si nécessaire
THREADS=(1 4 8 16)             # tests
SCHEDULES=("static" "dynamic")
CHUNKS=(1 4 16 32 64)          # essais de chunk sizes

# If you want to limit runs for speed, comment out some sizes

for N in "${SIZES[@]}"; do
  for sched in "${SCHEDULES[@]}"; do
    # for sequential baseline we always run 1-thread (will be appended by the first loop)
    for t in "${THREADS[@]}"; do
      if [ "$sched" == "static" ]; then
        # For static we still try chunks
        for chunk in "${CHUNKS[@]}"; do
          echo "Running N=$N threads=$t sched=$sched chunk=$chunk"
          $EXE $N $t $sched $chunk $OUT
        done
      else
        # dynamic
        for chunk in "${CHUNKS[@]}"; do
          echo "Running N=$N threads=$t sched=$sched chunk=$chunk"
          $EXE $N $t $sched $chunk $OUT
        done
      fi
    done
  done
done

echo "All tests done. Results in $OUT"

echo "Génération du plot"
python3 plot_results.py
echo "Plot généré avec succès."

