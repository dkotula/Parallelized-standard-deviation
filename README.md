# Parallelized-standard-deviation

To run this project:

```
$ gcc -fopenmp -o omp OpenMP.c
$ ./omp
```

```
$ mpicc MPI.c -o mpi
$ mpirun -np 4 ./mpi
```

```
$ mpicc -fopenmp hybrid.c -o hybrid
$ mpirun -np 4 ./hybrid
```
