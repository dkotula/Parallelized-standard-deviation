#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[])
{
    int i, n, N, numberCopy, sumOfDigits;
    unsigned long long int sum = 0;
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

    n = 0;
    omp_set_num_threads(8);
    start = omp_get_wtime();

    #pragma omp parallel for reduction(+ : sum) reduction(+ : n) private(numberCopy, sumOfDigits)
    for (i = 0; i < N; i++)
    {
        sumOfDigits = 0;
        numberCopy = A[i];

        while (numberCopy > 0)
        {
            sumOfDigits += numberCopy % 10;
            numberCopy /= 10;
        }

        if (sumOfDigits > 35)
        {
            sum += A[i];
            n++;
        }
    }

    if (n > 0)
    {
        sd = sum / n;
    }

    end = omp_get_wtime();
    printf("sum: %llu\nn: %d\n", sum, n);
    printf("Standard deviation: %Lf\nTime: %f\n", sd, end - start);

    free(A);

    return 0;
}