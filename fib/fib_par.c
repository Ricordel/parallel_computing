#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int fib(int n)
{
        int a, b;

        if (n < 2) {
                return n;
        }


#pragma omp task shared(a)
        a = fib(n-1);

#pragma omp task shared(b)
        b = fib(n-2);

#pragma omp taskwait
        return a + b;
}


int main(int argc, char *argv[])
{
        int rslt;
        int n;

        if (argc != 2) {
                printf("Usage: %s index\n", argv[0]);
                exit(1);
        }

        n = atoi(argv[1]); // unsafe


        // Only one thread must do this, but must be recursively parallel with tasks.
#pragma omp single
#pragma omp parallel
        rslt = fib(n);

        printf("%d\n", rslt);

        return 0;
}
