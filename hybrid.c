#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int i, n, N, numberCopy, sumOfDigits, rank, size, indices[2], index;
    unsigned long long int sum[2], sum_temp;
    long double sd = 0.0;
    double start, end;

    FILE *f = fopen("numbers.txt", "r");

    if (argc < 2)
    {
        N = 1000000;
    }
    else
    {
        N = atoi(argv[1]);
    }

    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    int *A = malloc(sizeof(int) * N);
    if (A == NULL)
    {
        printf("Unable to allocate memory!\n");
        exit(2);
    }

    for (i = 0; i < N; i++)
    {
        fscanf(f, "%d", &A[i]);
    }

    fclose(f);

    start = MPI_Wtime();
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0)
    {
        int interval = N / (size - 1);
        for (i = 1; i < size; i++)
        {
            indices[0] = (i - 1) * interval;
            indices[1] = i * interval;
            if (i == size - 1)
            {
                indices[1] = N;
            }
            MPI_Send(indices, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        n = 0;
        sum_temp = 0;
        for (i = 1; i < size; i++)
        {
            MPI_Recv(&sum, 2, MPI_UNSIGNED_LONG_LONG, i, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
            sum_temp += sum[0];
            n += sum[1];
        }

        if (n > 0)
        {
            sd = sum_temp / n;
        }

        end = MPI_Wtime();
        printf("sum: %llu\nn: %d\n", sum_temp, n);
        printf("Standard deviation: %Lf\nTime: %f\n", sd, end - start);
    }
    else
    {
        n = 0;
        sum_temp = 0;
        MPI_Recv(indices, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
        
        #pragma omp parallel for reduction(+ : sum_temp) reduction(+ : n) private(numberCopy, sumOfDigits)
        for (index = indices[0]; index < indices[1]; index++)
        {
            sumOfDigits = 0;
            numberCopy = A[index];

            while (numberCopy > 0)
            {
                sumOfDigits += numberCopy % 10;
                numberCopy /= 10;
            }

            if (sumOfDigits > 35)
            {
                sum_temp += A[index];
                n++;
            }
        }
        sum[0] = sum_temp;
        sum[1] = n;
        MPI_Send(sum, 2, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    free(A);

    return 0;
}