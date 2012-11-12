#include <stdio.h>
#include <stdlib.h>

#include "dbg.h"

/********* Just some quick testing, and a lot of unsafe stuff here. *************/

struct cell {
        int val;
        struct cell *pNext;
};


struct cell *list_from_array(int *array, int length)
{
        int i;
        struct cell *pHead = NULL;

        if (length == 0) {
                return pHead;
        }

        pHead = calloc (1, sizeof(struct cell));
        pHead->val = array[0];

        for (i = 1; i < length; i++) {
                struct cell *pCell = calloc (1, sizeof(struct cell));
                pCell->val = array[i];
                pCell->pNext = pHead;
                pHead = pCell;
        }

        return pHead;
}


int fib(int n)
{
        int i;
        int a = 0, b = 1;

        debug("Enters fib(%d)\n", n);

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
        int i;
        int run_parallel;
        int n_elem = 0;
        struct cell *pList = NULL;
        struct cell *pCurCell = NULL;

        if (argc != 3) {
                puts("Usage: parallel_map nIterations parallel?");
                exit(1);
        }

        n_elem = atoi(argv[1]); // Unsafe !
        run_parallel = atoi(argv[2]); // Unsafe !

        printf("n_elem: %d, run_parallel: %d\n", n_elem, run_parallel);

        int *array = calloc(n_elem, sizeof(int));
        volatile int *fibs = calloc(n_elem, sizeof(int));

        for (i = 0; i < n_elem; i++) {
                array[i] = 10*i;
        }

        pList = list_from_array(array, n_elem);
        check(pList != NULL, "pList should not be NULL here");
        check(pList->pNext != NULL, "pList->pNext should not be NULL here");

        /* Now the actual traversal and computation of fib */
        if (!run_parallel) {
                i = 0;
                for (pCurCell = pList; pCurCell != NULL; pCurCell = pCurCell->pNext) {
                        fibs[i] = fib(pCurCell->val);
                        i++;
                }
        } else {

#pragma omp parallel
#pragma omp single private(i, pCurCell)
        {
                pCurCell = pList;
                i = 0;
                while (pCurCell != NULL) {
                        debug("Tour de boucle");
                        // Générer une tâche pour le traitement de val. Elle sera exécutée plus tard.
                        // Une task "capture" son environnement, comme une closure en fait... Donc
                        // c'est comme si pCurCell et i étaient firstprivate dans la tâche.
                        #pragma omp task 
                                fibs[i] = fib(pCurCell->val);
                        // Puis avancer pour aller générer la prochaine tâche
                        i++;
                        pCurCell = pCurCell->pNext;
                }
                debug ("après la boucle");
        } // omp single
        } // else (!run_parallel)

error:
        /* XXX Should free some memory here, don't care. */
        return 0;
}

