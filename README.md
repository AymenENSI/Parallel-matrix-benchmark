# TP : Multiplication de matrices (parallélisation)

Ce dépôt contient un TP de multiplication de matrices carrées N x N avec une version séquentielle
et une version parallèle (pthread/OpenMP-like scheduling simulé via pthreads).

## Contenu

- `main.c` : programme principal qui exécute la multiplication séquentielle et parallèle, mesure les
  temps et écrit les résultats dans un fichier CSV.
- `matmul.c`, `matmul.h` : implémentations des fonctions de multiplication (séquentielle et
  parallèle).
- `utils.c`, `utils.h` : fonctions utilitaires (allocation, initialisation, chronométrage).
- `Makefile` : règles de compilation.
- `run_tests.sh` : script pour lancer une série d'expérimentations et créer/rafraîchir `results.csv`.
- `plot_result.py` : script Python pour tracer les résultats (speedup / efficiency) à partir de `results.csv`.
- `*.png` : images résultats générées par `plot_result.py`.

## But du TP

1. Étudier les performances de la multiplication de matrices.
2. Mesurer le speedup et l'efficacité en variant : la taille `N`, le nombre de threads `nthreads`,
   le type de scheduling (`static` ou `dynamic`) et la taille de `chunk`.
3. Tracer les résultats et analyser le comportement en fonction du parallélisme et du découpage.

## Compilation

Pour compiler le projet, depuis la racine du dépôt :

```bash
make
```

La cible compilera l'exécutable `matrix_mult`.

## Exécution

Usage :

```text
./matrix_mult N nthreads schedule(static|dynamic) chunk out_csv

- N : dimension des matrices (N x N)
- nthreads : nombre de threads (1 pour exécution séquentielle)
- schedule : "static" ou "dynamic"
- chunk : taille de chunk (entier > 0). Pour `static`, cela définit un bloc de lignes.
- out_csv : chemin vers le fichier CSV où append les résultats (utiliser "-" pour ne pas écrire)

Exemple :
./matrix_mult 512 8 static 16 results.csv
```

Le programme écrit une ligne CSV avec le format suivant :

```
N,nthreads,schedule,chunk,seq_time,par_time,speedup,efficiency
```

`run_tests.sh` contient un ensemble d'expérimentations typiques et s'occupe de réinitialiser
`results.csv` avant d'appender les résultats.

## Génération des graphiques

Un script Python `plot_result.py` est fourni pour tracer les résultats. Il lit `results.csv`
et produit des fichiers PNG (speedup_*.png, efficiency_*.png).

Pour tracer après avoir généré (ou mis à jour) `results.csv` :

```bash
python3 plot_result.py
```

Assurez-vous d'avoir les dépendances Python nécessaires (matplotlib, pandas). Si nécessaire,
installez-les avec :

```bash
python3 -m pip install --user matplotlib pandas
```

## Tests et vérification rapide

1. Compiler : `make`
2. Lancer un test séquentiel : `./matrix_mult 256 1 static 1 -`
3. Lancer un test parallèle : `./matrix_mult 256 4 static 16 results.csv`
4. Tracer les résultats : `python3 plot_result.py`

## Remarques pédagogiques

- Vérifier la reproductibilité : les matrices sont initialisées avec des seeds fixes (`1234`, `4321`).
- Le temps séquentiel est mesuré même quand on demande une exécution parallèle, il sert de base
  pour le calcul du speedup.

## Nettoyage

```bash
make clean
```

Cela supprime les objets, l'exécutable et `results.csv`.

## Aide / Problèmes

Si vous rencontrez des problèmes de compilation ou d'exécution, partagez l'erreur complète.
Vérifiez que `gcc` est installé et que vous utilisez un système Linux/Unix compatible.
