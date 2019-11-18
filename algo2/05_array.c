#include "def.h"
#include "05_array.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define ARRAY_INIT_SIZE		10

// 匹配函数sample
int array_match_sample(void *a, void *b)
{
	int *left, *right;

	if (a == NULL || b == NULL)
	{
		return -EINVAL;
	}

	left = (int*)a;
	right = (int*)b;

	return *left - *right;
}

// 打印函数sample
int array_print_elem_sample(void *arg)
{
	int *val = (int*)arg;

	if (val == NULL)
	{
		return -EINVAL;
	}

	int ret = printf("%d", *val);
	return ret;
}

// pArr：数组对象指针
// size：数组初始大小，为0时使用默认大小10
// ele_size：元素类型大小
// free_elem：元素释放函数，NULL表示不需要对元素进行单独释放
// match：元素比较函数，比如array_match_sample
// print_elem：元素打印函数，比如array_print_elem_sample
int array_init(PARRAY pArr, int size, int ele_size,
	void(*free_elem)(void*), int(*match)(void*, void*), void(*print_elem)(void*))
{
	if (pArr == NULL || size < 0 || ele_size <= 0)
	{
		return -EINVAL;
	}

	int init_size = ARRAY_INIT_SIZE;
	if (size > 0)
		init_size = size;

	pArr->pElem = malloc(init_size * ele_size);
	if (pArr->pElem == NULL)
	{
		return -ENOMEM;
	}
	memset(pArr->pElem, 0, init_size * ele_size);

	pArr->size = init_size;
	pArr->count = 0;
	pArr->ele_size = ele_size;

	pArr->free_elem = free_elem;
	pArr->match = match;
	pArr->print_elem = print_elem;

	return 0;
}

int array_fin(PARRAY pArr)
{
	int i;
	void *pElem = NULL;

	if (pArr == NULL || pArr->pElem == NULL)
		return 0;

	if (pArr->free_elem != NULL)
	{
		for (i = 0; i < pArr->count; i++)
		{
			pElem = array_get(pArr, i);
			pArr->free_elem(pElem);
		}
	}

	free(pArr->pElem);

	pArr->pElem = NULL;
	pArr->size = pArr->count = pArr->ele_size = 0;
	pArr->free_elem = NULL;
	pArr->match = NULL;
	pArr->print_elem = NULL;

	return 0;
}

void* array_get(PARRAY pArr, int i)
{
	if (pArr == NULL || pArr->pElem == NULL || i < 0 || i >= pArr->count)
	{
		return NULL;
	}

	return (void*)((PBYTE)(pArr->pElem) + i * pArr->ele_size);
}

void* array_add(PARRAY pArr, void *arg)
{
	if (pArr == NULL || arg == NULL)
	{
		//return -EINVAL;
		return NULL;
	}

	if (pArr->count >= pArr->size)
	{
		int new_size = pArr->size * 2;

		void *p_new_buf = realloc(pArr->pElem, new_size * pArr->ele_size);
		if (p_new_buf == NULL)
		{
			printf("[array_add]realloc fail.[size:%d-%d]\n", pArr->size, new_size);
			//return -ENOMEM;
			return NULL;
		}

		pArr->pElem = p_new_buf;
		pArr->size = new_size;
	}

	PBYTE p_new_elem = (PBYTE)(pArr->pElem) + pArr->count * pArr->ele_size;
	memcpy(p_new_elem, arg, pArr->ele_size);
	pArr->count++;

	return (void*)p_new_elem;
}

int array_del(PARRAY pArr, int i)
{
	void *find = array_get(pArr, i);
	if (find == NULL)
	{
		printf("[array_del]not found.\n");
		return 0;
	}

	if (pArr->free_elem != NULL)
		pArr->free_elem(find);

	if (pArr->count == 1)
	{
		pArr->count = 0;
	}
	else if (pArr->count == i + 1)
	{
		pArr->count--;
	}
	else
	{
		PBYTE p_elem_last = (PBYTE)(pArr->pElem) + (pArr->count - 1) * pArr->ele_size;
		memcpy(find, p_elem_last, pArr->ele_size);

		pArr->count--;
	}

	return 1;
}

int array_search(PARRAY pArr, void *arg)
{
	int ret = -1;

	if (pArr == NULL || arg == NULL)
		return -EINVAL;

	if (pArr->match == NULL)
	{
		printf("[array_search]Warning: To use array_search, you must use array_init to init match function first!\n");
		return -EINVAL;
	}

	int i;
	void *p_elem;
	for (i = 0; i < pArr->count; i++)
	{
		p_elem = array_get(pArr, i);
		if (p_elem != NULL && pArr->match(arg, p_elem) == 0)
		{
			ret = i;
			break;
		}
	}

	return ret;
}

int array_update(PARRAY pArr, int i, void *arg)
{
	if (pArr == NULL || i < 0 || i >= pArr->count || arg == NULL)
	{
		printf("[array_update]invalid param.\n");

		return -EINVAL;
	}

	void *pElem = array_get(pArr, i);
	if (pElem == NULL)
	{
		printf("[array_update]not found.\n");

		return -1;
	}

	if (pArr->free_elem != NULL)
	{
		pArr->free_elem(pElem);
	}

	memcpy(pElem, arg, pArr->ele_size);

	return 1;
}

/////////////////////////以下为升序数组操作函数/////////////////////

void* array_seq_add(PARRAY pArr, void *arg)
{
	if (pArr == NULL || arg == NULL)
	{
		return NULL;
	}

	if (pArr->match == NULL)
	{
		printf("[array_seq_add]Warning: To use array_seq_add, you must use array_init to init match function first!\n");
		return NULL;
	}

	if (pArr->count >= pArr->size)
	{
		int new_size = pArr->size * 2;

		void *p_new_buf = realloc(pArr->pElem, new_size * pArr->ele_size);
		if (p_new_buf == NULL)
		{
			printf("[array_add]realloc fail.[size:%d-%d]\n", pArr->size, new_size);

			return NULL;
		}

		pArr->pElem = p_new_buf;
		pArr->size = new_size;
	}

	int pos;
	void *pElem;
	for (pos = 0; pos < pArr->count; pos++)
	{
		pElem = array_get(pArr, pos);

		if (pArr->match(arg, pElem) < 0)
		{
			break;
		}
	}

	if (pos < pArr->count)
	{
		memmove((PBYTE)pArr->pElem + pArr->ele_size * (pos + 1), (PBYTE)pArr->pElem + pArr->ele_size * pos,
			pArr->ele_size * (pArr->count - pos));
	}

	PBYTE p_new_elem = (PBYTE)pArr->pElem + pArr->ele_size * pos;
	memcpy(p_new_elem, arg, pArr->ele_size);
	pArr->count++;

	return (void*)p_new_elem;
}

int array_seq_del(PARRAY pArr, void *arg)
{
	if (pArr == NULL || arg == NULL)
	{
		return -1;
	}

	if (pArr->match == NULL)
	{
		printf("[array_seq_del]Warning: To use array_seq_add, you must use array_init to init match function first!\n");
		return -1;
	}

	int n_find = array_search(pArr, arg);
	if (n_find < 0)
	{
		printf("[array_seq_del]not found.\n");
		return 0;
	}

	void *pElem = array_get(pArr, n_find);
	if (pArr->free_elem != NULL)
		pArr->free_elem(pElem);

	if (pArr->count == 1)
	{
		pArr->count = 0;
	}
	else if (pArr->count == n_find + 1)
	{
		pArr->count--;
	}
	else
	{
		memmove((PBYTE)pArr->pElem + pArr->ele_size * n_find, (PBYTE)pArr->pElem + pArr->ele_size * (n_find + 1),
			pArr->ele_size * (pArr->count - n_find - 1));

		pArr->count--;
	}

	return 1;
}

int array_seq_update(PARRAY pArr, int i, void *arg)
{
	if (pArr == NULL || i < 0 || i >= pArr->count || arg == NULL)
	{
		printf("[array_seq_update]invalid param.\n");

		return -EINVAL;
	}

	if (pArr->match == NULL)
	{
		printf("[array_seq_update]Warning: To use array_seq_add, you must use array_init to init match function first!\n");
		return -1;
	}

	void *pElem = array_get(pArr, i);
	if (pArr->free_elem != NULL)
	{
		pArr->free_elem(pElem);
	}

	int pos = i;
	while (pos - 1 >= 0 && pArr->match(arg, array_get(pArr, pos - 1)) < 0)
	{
		pos--;
	}

	if (pos == i)
	{
		while (pos + 1 < pArr->count && pArr->match(arg, array_get(pArr, pos + 1)) > 0)
		{
			pos++;
		}
	}

	if (pos < i)
	{
		memmove((PBYTE)pArr->pElem + pArr->ele_size * i, (PBYTE)pArr->pElem + pArr->ele_size * pos, i - pos);
	}
	else if (pos > i)
	{
		memmove((PBYTE)pArr->pElem + pArr->ele_size * i, (PBYTE)pArr->pElem + pArr->ele_size * pos, pos - i);
	}

	memcpy(pElem, arg, pArr->ele_size);

	return 1;
}