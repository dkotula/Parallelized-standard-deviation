#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[])
{
    int i, n;

    if (argc < 2)
    {
        n = 1000000;
    }
    else
    {
        n = atoi(argv[1]);
    }

    FILE *f = fopen("numbers.txt", "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }

    srand(time(NULL));
    for (i = 0; i < n; i++)
    {
        fprintf(f, "%d\n", rand() % 900000 + 100000);
    }

    fclose(f);
    return 0;
}