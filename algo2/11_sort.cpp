#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>

#include "11_sort.h"

//
// swap
//
void swap(int a[], int i, int j)
{
	int tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

//
// bubble sort
//
void bubble_sort(int a[], int n)
{
	if (n <= 1)
		return;

	for (int i = 0; i < n - 1; i++)
	{
		int flag = 1;

		for (int j = 0; j < n - i - 1; j++)
		{
			if (a[j] > a[j + 1])
			{
				flag = 0;

				swap(a, j, j + 1);
			}
		}

		if (flag)
			break;
	}
}

//
// selection sort
//
void selection_sort(int a[], int n)
{
	if (n <= 1)
		return;

	for (int i = 0; i < n - 1; i++)
	{
		int min_index = i;
		for (int j = i + 1; j < n; j++)
		{
			if (a[j] < a[min_index])
			{
				min_index = j;
			}
		}

		swap(a, i, min_index);
	}
}

//
// insertion sort
//
void insertion_sort(int a[], int n)
{
	if (n <= 1)
		return;

	for (int i = 1; i < n; i++)
	{
		int inserted = a[i];
		int j;

		for (j = i - 1; j >= 0; j--)
		{
			if (a[j] <= inserted)
				break;

			a[j + 1] = a[j];
		}

		a[j + 1] = inserted;
	}
}

//
// shell sort
//
void shell_insertion_sort(int a[], int start, int n, int gap)
{
	for (int i = start + gap; i < n; i += gap)
	{
		int inserted = a[i];
		int j;

		for (j = i - gap; j >= 0; j -= gap)
		{
			if (a[j] <= inserted)
				break;

			a[j + gap] = a[j];
		}

		a[j + gap] = inserted;
	}
}
void shell_sort2(int a[], int n)
{
	if (n <= 1)
		return;

	for (int gap = n / 2; gap >= 1; gap /= 2)
	{
		for (int i = 0; i < gap; i++)
			shell_insertion_sort(a, i, n, gap);
	}
}
void shell_sort(int a[], int n)
{
	if (n <= 1)
		return;

	for (int step = n / 2; step >= 1; step /= 2)
	{
		for (int i = step; i < n; i++)
		{
			int inserted = a[i];
			int j;

			for (j = i - step; j >= 0; j -= step)
			{
				if (a[j] > inserted)
				{
					a[j + step] = a[j];
				}
				else
				{
					break;
				}
			}

			a[j + step] = inserted;
		}
	}
}

//
// merge sort
//
void merge(int a[], int p, int q, int r)
{
	int flen = q - p + 1;
	int llen = r - q;

	int *fa = (int *)malloc(sizeof(int) * (flen + 1));
	int *la = (int *)malloc(sizeof(int) * (llen + 1));
	if (fa == NULL || la == NULL)
	{
		perror("[merge_sort_c]malloc failed.");
		exit(1);
	}

	memcpy(fa, a + p, sizeof(int) * flen);
	memcpy(la, a + q + 1, sizeof(int) * llen);
	fa[flen] = la[llen] = INT_MAX;

	for (int iter = p, i = 0, j = 0; iter <= r; iter++)
	{
		if (fa[i] <= la[j])
		{
			a[iter] = fa[i++];
		}
		else
		{
			a[iter] = la[j++];
		}
	}

	free(fa);
	free(la);
}
void merge_sort_c(int a[], int p, int r)
{
	if (p >= r)
		return;

	int q = (p + r) / 2;
	
	merge_sort_c(a, p, q);
	merge_sort_c(a, q + 1, r);

	merge(a, p, q, r);
}
void merge_sort(int a[], int n)
{
	if (n <= 1)
		return;

	merge_sort_c(a, 0, n - 1);
}

//
// quick sort
//
int partition(int a[], int p, int r)
{
	int pivot = a[r];

	int i = p;
	for (int j = p; j < r; j++)
	{
		if (a[j] < pivot)
		{
			if (i != j)
				swap(a, i, j);

			i++;
		}
	}

	swap(a, i, r);

	return i;
}
void quick_sort_c(int a[], int p, int r)
{
	if (p >= r)
		return;

	int q = partition(a, p, r);

	quick_sort_c(a, p, q - 1);
	quick_sort_c(a, q + 1, r);
}
void quick_sort(int a[], int n)
{
	quick_sort_c(a, 0, n - 1);
}

//
// find Nth max
//
int find_Nth_max_c(int a[], int p, int r, int nth)
{
	int pivot = a[r];
	int i, j;

	for (i = j = p; j < r; j++)
	{
		if (a[j] > pivot)
		{
			swap(a, i, j);
			i++;
		}
	}
	
	swap(a, i, r);

	if (i == nth - 1)
		return a[i];
	else if (i < nth - 1)
		return find_Nth_max_c(a, i + 1, r, nth);
	else
		return find_Nth_max_c(a, p, i - 1, nth);
}
int find_Nth_max(int a[], int n, int nth)
{
	if (n <= 0 || nth <= 0 || n < nth)
	{
		printf("Invalid parameters.\n");
		return -1;
	}

	int ret = find_Nth_max_c(a, 0, n - 1, nth);
	printf("%dth max num is %d\n", nth, ret);
	
	return 0;
}
/*
int main()
{
	//int a[] = {3, 4, 8, 7, 2, 5, 1, 6};
	int a[] = { 1, 1, 2};
	
	//bubble_sort(a, sizeof(a) / sizeof(int));
	//selection_sort(a, sizeof(a) / sizeof(int));
	//insertion_sort(a, sizeof(a) / sizeof(int));
	//shell_sort(a, sizeof(a) / sizeof(int));
	//shell_sort2(a, sizeof(a) / sizeof(int));
	//merge_sort(a, sizeof(a) / sizeof(int));
	//quick_sort(a, sizeof(a) / sizeof(int));

	//find_Nth_max(a, sizeof(a) / sizeof(int), 2);
	//find_Nth_max(a, sizeof(a) / sizeof(int), 5);
	//find_Nth_max(a, sizeof(a) / sizeof(int), 1);
	//find_Nth_max(a, sizeof(a) / sizeof(int), 4);

	find_Nth_max(a, sizeof(a) / sizeof(int), 3);

	for (int i = 0; i < sizeof(a) / sizeof(int); i++)
		printf("%d ", a[i]);
	printf("\n");
	
	return 1;
}
*/