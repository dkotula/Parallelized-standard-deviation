#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int i, n, N, numberCopy, sumOfDigits;
    unsigned long long int sum = 0;
    long double mean = 0.0, sd = 0.0;
    clock_t start, end;
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

    start = clock();

    for (i = 0, n = 0; i < N; i++)
    {
        numberCopy = A[i];
        sumOfDigits = 0;

        while (numberCopy > 0)
        {
            sumOfDigits += numberCopy % 10;
            numberCopy /= 10;
        }

        if (sumOfDigits > 30)
        {
            sum += A[i];
            n++;
        }
    }

    mean = sum / n;

    sum = 0;
    for (i = 0; i < N; i++)
    {
        numberCopy = A[i];
        sumOfDigits = 0;

        while (numberCopy > 0)
        {
            sumOfDigits += numberCopy % 10;
            numberCopy /= 10;
        }

        if (sumOfDigits > 30)
        {
            sum += (A[i] - mean) * (A[i] - mean);
        }
    }

    sd = sqrt(sum);

    end = clock();
    printf("sum: %llu\nn: %d\n", sum, n);
    printf("Standard deviation: %Lf\nTime: %f\n", sd, ((float)(end - start)) / CLOCKS_PER_SEC);

    free(A);

    return 0;
}