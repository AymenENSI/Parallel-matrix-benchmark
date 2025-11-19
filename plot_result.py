import pandas as pd
import matplotlib.pyplot as plt

# Charger le CSV
df = pd.read_csv("results.csv")

# Liste des tailles de matrices
matrix_sizes = df['N'].unique()
schedules = df['schedule'].unique()

for N in matrix_sizes:
    plt.figure(figsize=(10,6))
    
    for sched in schedules:
        subset = df[(df['N'] == N) & (df['schedule'] == sched)]
        
        # Pour speedup
        # On prend la moyenne du speedup sur tous les chunks pour chaque nombre de threads
        speedup = subset.groupby('threads')['speedup'].mean()
        plt.plot(speedup.index, speedup.values, marker='o', label=f"{sched} schedule")
    
    plt.title(f"Speedup vs Threads for Matrix Size {N}")
    plt.xlabel("Number of Threads")
    plt.ylabel("Speedup")
    plt.grid(True)
    plt.legend()
    plt.xticks(speedup.index)
    plt.savefig(f"speedup_N{N}.png")
    plt.show()

# Même chose pour l'efficacité
for N in matrix_sizes:
    plt.figure(figsize=(10,6))
    
    for sched in schedules:
        subset = df[(df['N'] == N) & (df['schedule'] == sched)]
        efficiency = subset.groupby('threads')['efficiency'].mean()
        plt.plot(efficiency.index, efficiency.values, marker='o', label=f"{sched} schedule")
    
    plt.title(f"Efficiency vs Threads for Matrix Size {N}")
    plt.xlabel("Number of Threads")
    plt.ylabel("Efficiency")
    plt.grid(True)
    plt.legend()
    plt.xticks(efficiency.index)
    plt.savefig(f"efficiency_N{N}.png")
    plt.show()
