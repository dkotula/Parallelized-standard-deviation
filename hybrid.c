#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int i, n;
    FILE *f = fopen("numbers.txt", "r");

    if (argc < 2)
    {
        n = 1000000;
    }
    else
    {
        n = atoi(argv[1]);
    }

    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int *A = malloc(sizeof(int) * n);
    if (A == NULL)
    {
        printf("Unable to allocate memory!\n");
        exit(2);
    }

    for (i = 0; i < n; i++)
    {
        fscanf(f, "%d", &A[i]);
    }

    fclose(f);

    free(A);

    return 0;
}