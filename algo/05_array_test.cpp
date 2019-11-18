#include "05_array.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int i, j;
	int *p_elem, *p_elem_a, *p_elem_b;

	int a[14] = { 4, 3, 7, 9, 10, 1,
		14, 3, 5, 11, 24, 99,
		2, 22 };
	int b[6] = { 77, 11, 3, 7, 5, 13 };

	ARRAY arr_a = { 0 };
	ARRAY arr_b = { 0 };
	ARRAY arr_total = { 0 };

	array_init(&arr_a, 0, sizeof(int), NULL, array_match_sample, NULL);
	array_init(&arr_b, 0, sizeof(int), NULL, array_match_sample, NULL);
	array_init(&arr_total, 0, sizeof(int), NULL, array_match_sample, NULL);

	// add and sort

	for (i = 0; i < 14; i++)
		array_seq_add(&arr_a, &a[i]);

	for (i = 0; i < 6; i++)
		array_seq_add(&arr_b, &b[i]);

	// merge

	i = j = 0;
	p_elem_a = (int*)array_get(&arr_a, i++);
	p_elem_b = (int*)array_get(&arr_b, j++);
	while (p_elem_a != NULL || p_elem_b != NULL)
	{
		if (p_elem_a == NULL)
		{
			array_add(&arr_total, p_elem_b);

			p_elem_b = (int*)array_get(&arr_b, j++);
		}
		else if (p_elem_b == NULL)
		{
			array_add(&arr_total, p_elem_a);

			p_elem_a = (int*)array_get(&arr_a, i++);
		}
		else
		{
			int cmp = array_match_sample(p_elem_a, p_elem_b);

			if (cmp < 0)
			{
				array_add(&arr_total, p_elem_a);

				p_elem_a = (int*)array_get(&arr_a, i++);
			}
			else
			{
				array_add(&arr_total, p_elem_b);

				p_elem_b = (int*)array_get(&arr_b, j++);
			}
		}
	}

	// print

	printf("arr_total:\n");
	for (i = 0; i < arr_total.count; i++)
	{
		p_elem = (int*)array_get(&arr_total, i);
		printf("[%d] ", *p_elem);
	}
	printf("\n");

	printf("arr_a:\n");
	for (i = 0; i < arr_a.count; i++)
	{
		p_elem = (int*)array_get(&arr_a, i);
		printf("[%d] ", *p_elem);
	}
	printf("\n");

	printf("arr_b:\n");
	for (i = 0; i < arr_b.count; i++)
	{
		p_elem = (int*)array_get(&arr_b, i);
		printf("[%d] ", *p_elem);
	}
	printf("\n");

	array_fin(&arr_total);
	array_fin(&arr_a);
	array_fin(&arr_b);

	return 1;
}