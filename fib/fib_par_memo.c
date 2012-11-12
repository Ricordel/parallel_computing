#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dbg.h"


struct padded_int {
        int val;
        char padding[64 - sizeof(int)];
};


int fib(int n, volatile struct padded_int *memo)
{
        int a, b;

        if (n < 2) {
                memo[n].val = n;
#pragma omp flush(memo)
                return n;
        }

        if (memo[n].val >= 0) {
                debug("Hit for %d\n", n);
                return memo[n].val;
        }

#pragma omp task shared(a, memo) if (memo[n-1].val < 0)
        if (memo[n-1].val >= 0) {
                a = memo[n-1].val;
        } else {
                debug("coupute for %d\n", n-1);
                a = fib(n-1, memo);
        }


#pragma omp task shared(b, memo) if (memo[n-2].val < 0)
        if (memo[n-2].val >= 0) {
                b = memo[n-2].val;
        } else {
                debug("coupute for %d\n", n-2);
                b = fib(n-2, memo);
        }

#pragma omp taskwait
        debug("Store result for %d\n", n);
        memo[n].val = a + b;

#pragma omp flush(memo)


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

        volatile struct padded_int memo[n+1];
        memset((void*)memo, -1, sizeof(memo));
        
        memo[0].val = 0;
        if (n > 1) {
                memo[1].val = 1;
        }

        // Only one thread must do this, but must be recursively parallel with tasks.
#pragma omp single
#pragma omp parallel shared(memo)
        rslt = fib(n, memo);

        printf("%d\n", rslt);

        return 0;
}
