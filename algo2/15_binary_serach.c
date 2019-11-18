#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>

#include "15_binary_search.h"
#include "def.h"

int bi_search(int a[], int size, int val)
{
	if (a == NULL || size == 0)
		return -EINVAL;

	int low = 0;
	int high = size - 1;
	while (low <= high)
	{
		int mid = low + ((high - low) >> 1);

		if (a[mid] > val)
			high = mid - 1;
		else if (a[mid] < val)
			low = mid + 1;
		else
			return mid;
	}

	return -1;
}

int bi_search_first(int a[], int size, int val)
{
	if (a == NULL || size == 0)
		return -EINVAL;

	int low = 0;
	int high = size - 1;
	int mid;
	while (low <= high)
	{
		mid = low + ((high - low) >> 1);

		if (a[mid] > val)
		{
			high = mid - 1;
		}
		else if (a[mid] < val)
		{
			low = mid + 1;
		}
		else
		{
			if (mid == 0 || a[mid - 1] != val)
				return mid;
			else
				high = mid - 1;
		}
	}

	return -1;
}

int bi_search_last(int a[], int size, int val)
{
	if (a == NULL || size == 0)
		return -EINVAL;

	int low = 0;
	int high = size - 1;
	int mid;
	while (low <= high)
	{
		mid = low + ((high - low) >> 1);

		if (a[mid] > val)
			high = mid - 1;
		else if (a[mid] < val)
			low = mid + 1;
		else
		{
			if (mid == size - 1 || a[mid + 1] != val)
				return mid;
			else
				low = mid + 1;
		}
	}

	return -1;
}

int bi_search_less_equal(int a[], int size, int val)
{
	if (a == NULL || size == 0)
		return -EINVAL;

	int low = 0;
	int high = size - 1;
	int mid;
	while (low <= high)
	{
		mid = low + ((high - low) >> 1);

		if (a[mid] > val)
			high = mid - 1;
		else if (a[mid] <= val)
		{
			if (mid == size - 1 || a[mid + 1] > val)
				return mid;
			else
				low = mid + 1;
		}
	}

	return -1;
}

int bi_search_great_equal(int a[], int size, int val)
{
	if (a == NULL || size == 0)
		return -EINVAL;

	int low = 0;
	int high = size - 1;
	int mid;
	while (low <= high)
	{
		mid = low + ((high - low) >> 1);

		if (a[mid] >= val)
		{
			if (mid == 0 || a[mid - 1] < val)
				return mid;
			else
				high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
	}

	return -1;
}

int bi_search_circular(int a[], int size, int val)
{
	if (a == NULL || size == 0)
		return -EINVAL;

	int low, high, mid, pivot;

	//
	// find the pivot
	//

	pivot = 0;
	if (a[0] > a[size - 1])
	{
		low = 0;
		high = size - 1;

		while (a[low] > a[high])
		{
			mid = low + ((high - low) >> 1);

			if (a[low] > a[mid])
			{
				if (a[mid - 1] > a[mid])
				{
					pivot = mid;
					break;
				}

				high = mid - 1;
			}
			else if (a[mid] > a[high])
			{
				if (a[mid] > a[mid + 1])
				{
					pivot = mid + 1;
					break;
				}

				low = mid + 1;
			}
			else
			{
				pivot = mid + 1;
				break;
			}
		}
	}

	//
	// binary search
	//

	//****
	printf("pivot:%d\t\t", pivot);

	low = 0;
	high = size - 1;
	
	while (low <= high)
	{
		mid = low + ((high - low) >> 1);

		int mid_real_index = (mid + pivot) % size;

		if (a[mid_real_index] > val)
		{
			high = mid - 1; 
		}
		else if (a[mid_real_index] < val)
		{
			low = mid + 1;
		}
		else
		{
			return mid_real_index;
		}
	}

	return -1;
}

void test_bi_search_class(char *name, int a[], int size, int (*bi_search_class)(int [], int, int))
{
	printf("%s(%d):\t%d\n", name, 5, bi_search_class(a, size, 5));
	printf("%s(%d):\t%d\n", name, 10, bi_search_class(a, size, 10));
	printf("%s(%d):\t%d\n", name, 6, bi_search_class(a, size, 6));
	printf("%s(%d):\t%d\n", name, -2, bi_search_class(a, size, -2));
	printf("%s(%d):\t%d\n", name, 15, bi_search_class(a, size, 15));
	printf("%s(%d):\t%d\n", name, 1, bi_search_class(a, size, 1));
	printf("%s(%d):\t%d\n", name, 13, bi_search_class(a, size, 13));
}
/*
int main()
{
#define ARR_SIZE	10
	int a[ARR_SIZE] = { 7, 8, 10, 13, 1, 2, 3, 4, 5, 6 };
	int b[ARR_SIZE + 1] = { 4, 5, 6, 7, 8, 10, 11, 13, 1, 2, 3 };

	printf("bi_search_circular:\n\n");
	test_bi_search_class("a", a, ARR_SIZE, bi_search_circular);
	printf("\n");
	test_bi_search_class("b", b, ARR_SIZE + 1, bi_search_circular);
	printf("\n");
}
*/
/*
int main()
{
#define ARR_SIZE	10
	int a[ARR_SIZE] = { 1, 2, 3, 4, 5, 5, 7, 8, 10, 13 };
	int b[ARR_SIZE + 1] = { 1, 2, 3, 4, 5, 5, 7, 8, 10, 11, 13 };

	//printf("a(%d):\t%d\n", 4, bi_search(a, ARR_SIZE, 4));
	//printf("a(%d):\t%d\n", 10, bi_search(a, ARR_SIZE, 10));
	//printf("a(%d):\t%d\n", 6, bi_search(a, ARR_SIZE, 6));
	//printf("a(%d):\t%d\n", -2, bi_search(a, ARR_SIZE, -2));
	//printf("a(%d):\t%d\n", 15, bi_search(a, ARR_SIZE, 15));
	//printf("a(%d):\t%d\n", 1, bi_search(a, ARR_SIZE, 1));
	//printf("a(%d):\t%d\n", 13, bi_search(a, ARR_SIZE, 13));

	printf("bi_search:\n\n");
	test_bi_search_class("a", a, ARR_SIZE, bi_search);
	printf("\n");
	test_bi_search_class("b", b, ARR_SIZE + 1, bi_search);
	printf("\n");

	printf("bi_search_first:\n\n");
	test_bi_search_class("a", a, ARR_SIZE, bi_search_first);
	printf("\n");
	test_bi_search_class("b", b, ARR_SIZE + 1, bi_search_first);
	printf("\n");

	printf("bi_search_last:\n\n");
	test_bi_search_class("a", a, ARR_SIZE, bi_search_last);
	printf("\n");
	test_bi_search_class("b", b, ARR_SIZE + 1, bi_search_last);
	printf("\n");

	printf("bi_search_less_equal:\n\n");
	test_bi_search_class("a", a, ARR_SIZE, bi_search_less_equal);
	printf("\n");
	test_bi_search_class("b", b, ARR_SIZE + 1, bi_search_less_equal);
	printf("\n");

	printf("bi_search_great_equal:\n\n");
	test_bi_search_class("a", a, ARR_SIZE, bi_search_great_equal);
	printf("\n");
	test_bi_search_class("b", b, ARR_SIZE + 1, bi_search_great_equal);
	printf("\n");

	return 0;
}
*/