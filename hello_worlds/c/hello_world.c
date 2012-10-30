#include <pthread.h>
#include <stdio.h>

#include "../../utils/my_utils.h"

#define NB_ITER 6



pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int count = 0;



void * print_and_increment(void *unused __attribute__((unused)))
{
        int count_loc;
        pthread_mutex_lock(&mutex);
        count_loc = count++;
        pthread_mutex_unlock(&mutex);

        printf("Iter: %d\n", count_loc);

        return NULL; /* Ignored */
}




int main(int argc, char *argv[]) {

        int i, j;
        int nThreads = 1;
        pthread_t *threadArray;

        /* This program accepts 0 or 1 arguments (argc==2). If one argument
           is used, it is interpreted as an integer and put in variable nThreads */

        if (argc == 2)
                nThreads = atoi(argv[1]);

        threadArray = xmalloc(nThreads * sizeof(pthread_t));

        // Make the code from here until the last printf-statement parallel
        printf("Hello World\n");

        int nConcurThreads = min(NB_ITER, nThreads);

        pdebug("nConcurThreads = %d\n", nConcurThreads);

        for (i = 0; i < NB_ITER; ) {
                for (j = 0; j < nConcurThreads; j++) {
                        pthread_create(&threadArray[j], NULL, print_and_increment, NULL);
                        i++;
                }
                for (j = 0; j < nConcurThreads; j++) {
                        pthread_join(threadArray[j], NULL);
                }

        }

        printf("GoodBye World\n");

        return 0;
}
