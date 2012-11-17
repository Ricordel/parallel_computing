/***** Tentative implementation of a quicksort with openmp task generation *******/

/* This must be compiled with a C99-compliant C compiler */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>

#include "dbg.h"


/** Check if an array is sorted or not
 * in: tab      The array to check
 * in: length   The length of this array
 *
 * return: true if the array is sorted, false otherwise
 */
bool is_sorted(int *tab, int length);


/** Partitions a section of an array by selecting the pivot randomly
 * in p      start position
 * in q      end position
 * inout tab  pointer to the array to partition
 * out pPivot The position of the pivot will be written here
 *
 * return 0 if successful, anything else otherwise
 */
int partition(int *tab, int p, int q, int *pPivot);


/** Quicksorts an array between two points
 * in tab      The array to sort
 * in p        Start position
 * in r        End position
 *
 * return 0 if successful, anything else otherwise
 */
int quicksort(int *tab, int p, int r);


/** Inits an array with integers from 0 to length-1 randomly shuffled.
 * in tab      The array to init
 * in length   Length of the array
 *
 * return 0 if successful, anything else otherwise
 */
int init_data(int *tab, int length);


/* Runs some tests
 * 
 * return 0 if ok, 1 if failed */
int run_tests(void);



/***************************** Start of implementation *****************************/

bool is_sorted(int *tab, int length)
{
        for (int i = 0; i < length - 1; i++) {
                if (tab[i] > tab[i+1]) {
                        return 0;
                }
        }

        return 1;
}



#define swap_int(tab, i, j) \
do { \
        int tmp = tab[i]; \
        tab[i] = tab[j]; \
        tab[j] = tmp; \
} while (0)




/* My version */
int partition(int *tab, int p, int r, int *pPivot)
{
        check_null(pPivot);
{
        /* Take the first element as pivot */
        int pivot = tab[p];
        int firstGTPivot = p + 1;
        int lastLEPivot  = r;


        for (;;) {
                /* Find the last element that is <= pivot, checking we do not go too far */
                while (tab[lastLEPivot] > pivot && lastLEPivot >= firstGTPivot) {
                        lastLEPivot--;
                }

                /* Find the first element that is > pivot, checking we do not go too far */
                while (tab[firstGTPivot] <= pivot && firstGTPivot <= lastLEPivot) {
                        firstGTPivot++;
                }

                /* Do we have to swap two values ? */
                if (firstGTPivot < lastLEPivot) {
                        swap_int(tab, firstGTPivot, lastLEPivot);

                } else { /* our array is partitioned, just put the pivot to the right place */

                        check(firstGTPivot == lastLEPivot + 1,
                              "firstGTPivot = %d, lastLEPivot = %d", firstGTPivot, lastLEPivot);
                        swap_int(tab, p, lastLEPivot);
                        *pPivot = lastLEPivot;
                        break;
                }
        }

        return 0;
error:
        return -1;
}
}



int quicksort(int *tab, int p, int r)
{
        int ret = 0;
	if (p < r) {
		int q;
                ret = partition(tab, p, r, &q);
                check (ret == 0, "error in partiton between %d and %d\n", p, r);

		ret = quicksort(tab, p, q - 1);
                check (ret == 0, "Error calling quicksort between %d and %d\n", p, q-1);

		ret = quicksort(tab, q + 1, r);
                check (ret == 0, "Error calling quicksort between %d and %d\n", q + 1, r);
	}

error:
        return ret;
}



int init_data(int *tab, int length)
{
	int i, x, y, t;
	for (i = 0; i < length; i++)
		tab[i] = i;

	for (i = 0; i < length; i++) {
		x = rand() % length;
		y = rand() % length;
		t = tab[y];
		tab[y] = tab[x];
		tab[x] = t;
	}

        return 0;
}




int main(int argc, char *argv[])
{
        int ret;
        int nItems;
        int *tab = NULL;

        if (argc != 2) {
                printf("Usage: %s nItems\n", argv[0]);
                goto error;
        }

        if (strcmp(argv[1], "tests") == 0) {
                return run_tests();
        }

        nItems = atoi(argv[1]); /* Unsafe ! */
        if (nItems < 0) {
                puts("The number of items must be > 0");
                exit(1);
        }

        /***** Allocate and initialize the array ******/

        tab = malloc(nItems * sizeof(int));
        check_mem(tab);

	ret = init_data(tab, nItems);
        check (ret == 0, "Failed to init data\n");

        double start_time = omp_get_wtime();
	quicksort(tab, 0, nItems - 1);
        double end_time = omp_get_wtime();

        if (!is_sorted(tab, nItems)) {
                log_err("the array is not sorted");
        } else {
                printf("{\"nElems\": %d, \"time\": %lf}\n", nItems, end_time - start_time);
        }

	return 0;
error:
        free(tab);
        exit(1);
}




int zero_length_test()
{
        puts("It should handle 0-length arrays");
        int dummy;
        int ret = quicksort(&dummy, 0, 0);
        check(ret == 0, "quicksort failed");

        return 0;
error:
        return -1;
}


int one_length_test()
{
        puts("It should handle 1-length arrays");
        int dummy = 9;
        int ret = quicksort(&dummy, 0, 0);
        check(ret == 0, "quicksort failed");
        if (dummy != 9) {
                return -1;
        }

        return 0;
error:
        return -1;
}


int two_length_test()
{
        puts("It should handle 2-length arrays");
        {
                int tab[2] = {0, 23};
                int ret = quicksort(tab, 0, 1);
                check(ret == 0, "quicksort failed");
                if (tab[0] > tab[1]) {
                        return -1;
                }
        }
        {
                int tab[2] = {23, 90};
                int ret = quicksort(tab, 0, 1);
                check(ret == 0, "quicksort failed");
                if (tab[0] > tab[1]) {
                        return -1;
                }
        }

        return 0;
error:
        return -1;
}


int already_sorted_test()
{
        puts("It should handle already sorted arrays");

        int tab[5] = {0, 1, 5, 6, 10};
        int ret = quicksort(tab, 0, 4);
        check(ret == 0, "Error while sorting");

        if (!is_sorted(tab, 5)) {
                return -1;
        }

        return 0;
error:
        return -1;
}


int reverse_sorted_test()
{
        puts("It should handle reverse sorted arrays");

        int tab[5] = {2345, 523, 98, 4, 2};
        int ret = quicksort(tab, 0, 4);
        check(ret == 0, "Error while sorting");

        if (!is_sorted(tab, 5)) {
                return -1;
        }

        return 0;
error:
        return -1;
}


#define RUN_TEST(test) \
do { \
        int ret = test(); \
        if (ret != 0) { \
                nb_failed++; \
                puts("\t FAILED"); \
        } else { \
                puts("\t OK"); \
        } \
} while (0)


int run_tests(void)
{
        int nb_failed = 0;

        RUN_TEST(zero_length_test);
        RUN_TEST(one_length_test);
        RUN_TEST(two_length_test);
        RUN_TEST(already_sorted_test);
        RUN_TEST(reverse_sorted_test);

        if (nb_failed == 0) {
                puts("ALL TESTS PASSED");
        } else {
                printf("%d TESTS FAILED\n", nb_failed);
        }

        return nb_failed;
}
