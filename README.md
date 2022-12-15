# Parallelized-standard-deviation

To run this project:

```
$ gcc -lm -o sd standardDeviation.c
$ ./sd
```

```
$ gcc -lm -fopenmp -o omp OpenMP.c
$ ./omp
```

```
$ mpicc -lm MPI.c -o mpi
$ mpirun -np 4 ./mpi
```

```
$ mpicc -lm -fopenmp hybrid.c -o hybrid
$ mpirun -np 4 ./hybrid
```
