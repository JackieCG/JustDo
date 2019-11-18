#include <stdio.h>

//
// 斐波那契数列
//

size_t Fibonacci_recursive(size_t n)
{
	if (n == 1)
		return 1;
	else if (n == 2)
		return 2;
	else
		return Fibonacci_recursive(n - 1) + Fibonacci_recursive(n - 2);
}

size_t Fibonacci_loop(size_t n)
{
	size_t n1, n2, sum;

	n1 = 0;
	n2 = 1;
	sum = 0;
	for (size_t i = 0; i < n; i++)
	{
		sum = n1 + n2;
		n1 = n2;
		n2 = sum;
	}

	return sum;
}

/*
int main()
{
	printf("F(%u):\t%u\t%u\n", 1, Fibonacci_recursive(1), Fibonacci_loop(1));
	printf("F(%u):\t%u\t%u\n", 2, Fibonacci_recursive(2), Fibonacci_loop(2));
	printf("F(%u):\t%u\t%u\n", 3, Fibonacci_recursive(3), Fibonacci_loop(3));
	printf("F(%u):\t%u\t%u\n", 4, Fibonacci_recursive(4), Fibonacci_loop(4));
	printf("F(%u):\t%u\t%u\n", 5, Fibonacci_recursive(5), Fibonacci_loop(5));
	printf("F(%u):\t%u\t%u\n", 6, Fibonacci_recursive(6), Fibonacci_loop(6));
	printf("F(%u):\t%u\t%u\n", 7, Fibonacci_recursive(7), Fibonacci_loop(7));

	return 0;
}
*/

//
// 阶乘
//

size_t factorial_recursive(size_t n)
{
	if (n == 0 || n == 1)
		return 1;
	
	return n * factorial_recursive(n - 1);
}

size_t factorial_loop(size_t n)
{
	size_t ret, i;

	for (ret = i = 1; i <= n; i++)
		ret *= i;

	return ret;
}

/*
int main()
{
	printf("F(%u):\t%u\t%u\n", 1, factorial_recursive(1), factorial_loop(1));
	printf("F(%u):\t%u\t%u\n", 2, factorial_recursive(2), factorial_loop(2));
	printf("F(%u):\t%u\t%u\n", 3, factorial_recursive(3), factorial_loop(3));
	printf("F(%u):\t%u\t%u\n", 4, factorial_recursive(4), factorial_loop(4));
	printf("F(%u):\t%u\t%u\n", 5, factorial_recursive(5), factorial_loop(5));
	printf("F(%u):\t%u\t%u\n", 6, factorial_recursive(6), factorial_loop(6));
	printf("F(%u):\t%u\t%u\n", 7, factorial_recursive(7), factorial_loop(7));

	return 0;
}
*/

//
// 全排列
//

void swap(int *a, int *b)
{
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

int full_permutaion_count = 0;

void full_permutaion_recursive(int arr[], size_t size, int sub_arr[], size_t sub_size)
{
	if (sub_size == 1)
	{
		printf("%d:\t", ++full_permutaion_count);

		for (size_t i = 0; i < size; i++)
			printf("[%d]", arr[i]);

		printf("\n");
	}
	else
	{
		for (size_t i = 0; i < sub_size; i++)
		{
			swap(sub_arr + 0, sub_arr + i);

			full_permutaion_recursive(arr, size, sub_arr + 1, sub_size - 1);

			swap(sub_arr + 0, sub_arr + i);
		}
	}
}
/*
int main()
{
#define ELEM_NUM	4

	int arr[ELEM_NUM] = { 1, 2, 3, 4 };
	full_permutaion_recursive(arr, ELEM_NUM, arr, ELEM_NUM);

	return 0;
}
*/