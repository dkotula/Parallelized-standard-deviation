#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int i, n;
    double start, end;
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

    start = omp_get_wtime();

    for (i = 0; i < n; i++)
    {
    }

    end = omp_get_wtime();
    printf("Time: %f\n", end - start);

    free(A);

    return 0;
}