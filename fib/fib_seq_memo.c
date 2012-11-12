#include <stdio.h>
#include <stdlib.h>

int fib(int n)
{
        int i;
        int a = 0, b = 1;

        if (n == 0) {
                return 0;
        }
        
        for (i = 0; i < n - 1; i++) {
                /* A l'entrée de cette boucle, a contient fib(i)
                 * b contient fib(i+1) */
                int tmp = b;
                b = a + b;
                a = tmp;
        }

        /* À la sortie de cette boucle, si n >= 1, i = n-1, donc par l'invariant,
         * a contient fib(n - 1 + 1) == fib(n) */

        return b;
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
        rslt = fib(n);

        printf("%d\n", rslt);

        return 0;
}
