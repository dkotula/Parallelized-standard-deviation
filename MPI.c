#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include <math.h>

int main(int argc, char *argv[])
{
    int i, n, N, numberCopy, sumOfDigits, rank, size, indices[2], index;
    unsigned long long int sum[2], sum_temp;
    long double mean = 0.0, sd = 0.0, indicesWithMean[3], sum_Sd;
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

        mean = sum_temp / n;

        for (i = 1; i < size; i++)
        {
            indicesWithMean[0] = (i - 1) * interval + 0.1;
            indicesWithMean[1] = i * interval + 0.1;
            indicesWithMean[2] = mean;
            if (i == size - 1)
            {
                indices[1] = N;
            }
            MPI_Send(indicesWithMean, 3, MPI_LONG_DOUBLE, i, 0, MPI_COMM_WORLD);
        }

        sum_Sd = 0.0;
        for (i = 1; i < size; i++)
        {
            MPI_Recv(&sd, 1, MPI_LONG_DOUBLE, i, MPI_ANY_TAG, MPI_COMM_WORLD, NULL);
            sum_Sd += sd;
        }

        sd = sum_Sd(sum);

        end = MPI_Wtime();
        printf("sum: %llu\nn: %d\n", sum_temp, n);
        printf("Standard deviation: %Lf\nTime: %f\n", sd, end - start);
    }
    else
    {
        n = 0;
        sum[0] = 0;
        sum[1] = 0;
        MPI_Recv(indices, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, NULL);
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
                sum[0] += A[index];
                sum[1]++;
            }
        }
        MPI_Send(sum, 2, MPI_UNSIGNED_LONG_LONG, 0, 0, MPI_COMM_WORLD);
        
        MPI_Recv(indicesWithMean, 3, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD, NULL);
        sum_Sd = 0.0;
        for (index = int(indicesWithMean[0]); index < int(indicesWithMean[1]); index++)
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
                sum_Sd += (A[i] - indicesWithMean[2]) * (A[i] - indicesWithMean[2]);
            }
        }
        MPI_Send(sum_Sd, 1, MPI_LONG_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    free(A);

    return 0;
}