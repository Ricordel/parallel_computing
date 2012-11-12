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


/* This is the reference implementation, not resonsible for it. */
int partition(int *tab, int p, int r, int *pPivot)
{
	int x = tab[p];
	int k = p;
	int l = r + 1;
	int t;

        check_null(pPivot);

	while (1) {
		do
			k++;
		while ((tab[k] <= x) && (k < r));
		do
			l--;
		while (tab[l] > x);

		while (k < l) {
			t = tab[k];
			tab[k] = tab[l];
			tab[l] = t;
			do
				k++;
			while (tab[k] <= x);
			do
				l--;
			while (tab[l] > x);
		}
		t = tab[p];
		tab[p] = tab[l];
		tab[l] = t;

                *pPivot = l;
                return 0;
	}

error:
        return -1;
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
                puts("ERROR: the array is not sorted");
        } else {
                printf("OK, array correctly sorted, in %lf sec\n", end_time - start_time);
        }

	return 0;
error:
        free(tab);
        exit(1);
}
