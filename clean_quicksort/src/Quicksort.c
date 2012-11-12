// Quicksort.cpp : Traditional version of Quicksort algorithm.  
//
//

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000000

int A[N];

int Partition(int p, int r)
{
	int x = A[p];
	int k = p;
	int l = r + 1;
	int t;
	while (1) {
		do
			k++;
		while ((A[k] <= x) && (k < r));
		do
			l--;
		while (A[l] > x);

		while (k < l) {
			t = A[k];
			A[k] = A[l];
			A[l] = t;
			do
				k++;
			while (A[k] <= x);
			do
				l--;
			while (A[l] > x);
		}
		t = A[p];
		A[p] = A[l];
		A[l] = t;
		return l;
	}
}

void QuickSort(int p, int r)
{
	if (p < r) {
		int q = Partition(p, r);
		QuickSort(p, q - 1);
		QuickSort(q + 1, r);
	}
}

void init_data()
{
	int i, x, y, t;
	for (i = 0; i < N; i++)
		A[i] = i;
	for (i = 0; i < N; i++) {
		x = rand() % N;
		y = rand() % N;
		t = A[y];
		A[y] = A[x];
		A[x] = t;
	}
}

int is_sorted(int *tab, int length)
{
        for (int i = 0; i < length - 1; i++) {
                if (tab[i] > tab[i+1]) {
                        return 0;
                }
        }

        return 1;
}





int main(void)
{
	init_data();

        double start_time = omp_get_wtime();
	QuickSort(0, N - 1);
        double end_time = omp_get_wtime();

        if (! is_sorted(A, N)) {
                puts("ERROR: the array is not sorted");
        } else {
                printf("OK, array correctly sorted, in %lf sec\n", end_time - start_time);
        }


	return 0;
}
