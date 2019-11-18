#ifndef _ARRAY_H
#define _ARRAY_H

typedef struct _ARRAY {
	void	*pElem;
	int		size;
	int		count;
	int		ele_size;

	int(*match)(void*, void*);	// 匹配函数
	void(*free_elem)(void*);	// 释放元素函数
	void(*print_elem)(void*);	// 打印函数
} ARRAY, *PARRAY;

int array_init(PARRAY pArr, int size, int ele_size,
	void(*free_elem)(void*), int(*match)(void*, void*), void(*print_elem)(void*));
int array_fin(PARRAY pArr);

void* array_get(PARRAY pArr, int i);
void* array_add(PARRAY pArr, void *arg);
int array_del(PARRAY pArr, int i);
int array_search(PARRAY pArr, void *arg);
int array_update(PARRAY pArr, int i, void *arg);

void* array_seq_add(PARRAY pArr, void *arg);
int array_seq_del(PARRAY pArr, void *arg);
int array_seq_update(PARRAY pArr, int i, void *arg);

int array_match_sample(void *a, void *b);
int array_print_elem_sample(void *arg);

#endif