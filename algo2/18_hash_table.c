#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <string.h>

#include "18_hash_table.h"
#include "def.h"

//
// 散列表冲突解决方法：链表法，使用单向链表实现
//

void ht_list_insert(ht_list_node *p_cur, htable_node *node)
{
	node->ht_list_node.next = p_cur->next;
	p_cur->next = node;
}

void ht_list_del(ht_list_node *prev, ht_list_node *cur)
{
	prev->next = cur->next;
}

int ht_list_delete(ht_list_node *head, int key)
{
	if (head == NULL)
		return -EINVAL;

	ht_list_node *prev;
	htable_node *cur_node;
	prev = head;
	cur_node = head->next;
	while (cur_node != NULL && cur_node->key != key)
	{
		prev = &cur_node->ht_list_node;
		cur_node = cur_node->ht_list_node.next;
	}

	if (cur_node != NULL)
	{
		ht_list_del(prev, &cur_node->ht_list_node);

		return 1;
	}

	return 0;
}

htable_node* ht_list_find(ht_list_node *head, int key)
{
	if (head == NULL)
		return NULL;

	htable_node *iter = head->next;
	while (iter != NULL && iter->key != key)
		iter = iter->ht_list_node.next;
	
	return iter;
}

int ht_list_traverse(ht_list_node *head)
{
	if (head == NULL)
		return -EINVAL;

	int i = 0;
	for (htable_node *iter = head->next; iter != NULL; iter = iter->ht_list_node.next)
	{
		printf("[%d: %d] ", ++i, iter->key);
	}
	printf("\n");

	return 1;
}

//
// 散列表
//

int htable_hash(htable *htable, int key)
{
	return (key * key / 2) % htable->size;
}

htable* htable_create(int size)
{
	htable *htable = malloc(sizeof(struct _htable));	// 这里不能用sizeof(htable)，否则sizeof会误以为求变量htable的大小
	if (htable == NULL)
		goto out;

	if (size < 10)
		size = 10;

	htable->hash_table = malloc(sizeof(ht_list_node) * size);
	if (htable->hash_table == NULL)
	{
		goto error;
	}

	memset(htable->hash_table, 0, sizeof(ht_list_node) * size);
	htable->size = size;

	goto out;

error:

	if (htable != NULL)
	{
		free(htable);
	}

	return NULL;

out:

	return htable;
}

int htable_destroy(htable *htable)
{
	if (htable == NULL)
		return -EINVAL;

	if (htable->hash_table != NULL)
		free(htable->hash_table);

	free(htable);

	return 0;
}

htable_node* htable_find(htable *htable, int key)
{
	if (htable == NULL)
		return NULL;

	int hash = htable_hash(htable, key);

	return ht_list_find(htable->hash_table + hash, key);
}

htable_node* htable_add_new(htable *htable, int key)
{
	if (htable == NULL)
		return NULL;

	htable_node *ht_node = htable_find(htable, key);
	if (ht_node == NULL)
	{
		htable_node *new_node = malloc(sizeof(htable_node));
		if (new_node == NULL)
			return NULL;

		memset(new_node, 0, sizeof(htable_node));
		new_node->key = key;

		int hash = htable_hash(htable, key);
		ht_list_insert(htable->hash_table + hash, new_node);
	}

	return ht_node;
}

int htable_add(htable *htable, htable_node *ht_node)
{
	if (htable == NULL || ht_node == NULL)
		return -EINVAL;

	int hash = htable_hash(htable, ht_node->key);
	ht_list_insert(htable->hash_table + hash, ht_node);

	return 0;
}

int htable_delete(htable *htable, int key)
{
	if (htable == NULL)
		return 0;

	int hash = htable_hash(htable, key);

	return ht_list_delete(htable->hash_table + hash, key);
}

void htable_traverse(htable *htable)
{
	if (htable == NULL)
		return;

	printf("\n======================= hash table ============================\n");

	for (int i = 0; i < htable->size; i++)
	{
		printf("%d:\t", i);
		ht_list_traverse(htable->hash_table + i);
	}

	printf("\n");

	return;
}

/*
int main()
{
#define ARRAY_SIZE	10
	int a[ARRAY_SIZE] = { 5, 15, 55, 33, 22, 44, 55, 22, 88, 25 };

	htable *htable = htable_create(10);
	if (htable == NULL)
	{
		printf("htable create fail.\n");
		exit(-1);
	}

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf("%d:\t[key:%d]\t", i, a[i]);

		htable_node *ht_node = htable_find(htable, a[i]);
		if (ht_node != NULL)
		{
			printf("already exist.\n");
		}
		else
		{
			ht_node = htable_add_new(htable, a[i]);
			if (ht_node != NULL)
				printf("add success.\n");
			else
				printf("add fail.\n");
		}
	}

	htable_traverse(htable);

	printf("hash table delete [%d]%s\n", 55, (htable_delete(htable, 55) ? "success" : "fail"));
	printf("hash table delete [%d]%s\n", 37, (htable_delete(htable, 37) ? "success" : "fail"));
	printf("hash table delete [%d]%s\n", 88, (htable_delete(htable, 88) ? "success" : "fail"));
	printf("hash table delete [%d]%s\n", 44, (htable_delete(htable, 44) ? "success" : "fail"));

	htable_traverse(htable);

	htable_destroy(htable);
	htable = NULL;

	return 0;
}
*/

//
// LRU缓存淘汰链表
//

//
// 双向循环链表
//

void lru_list_init(htable_node *head)
{
	head->lru_list_node.prev = head->lru_list_node.next = head;
}

void lru_list_insert(htable_node *p_cur, htable_node *p_elem)
{
	p_elem->lru_list_node.next = p_cur->lru_list_node.next;
	p_cur->lru_list_node.next = p_elem;
	p_elem->lru_list_node.prev = p_cur;
	p_elem->lru_list_node.next->lru_list_node.prev = p_elem;
}

void lru_list_delete(htable_node *cur)
{
	cur->lru_list_node.prev->lru_list_node.next = cur->lru_list_node.next;
	cur->lru_list_node.next->lru_list_node.prev = cur->lru_list_node.prev;
}

int lru_list_traverse(htable_node *lru_head)
{
	if (lru_head == NULL)
		return -EINVAL;

	//printf("=========================== lru list ===========================\n");

	int i = 0;
	for (htable_node *iter = lru_head->lru_list_node.next; iter != lru_head; iter = iter->lru_list_node.next)
	{
		printf("[%d: %d] ", ++i, iter->key);
	}

	//printf("\n\n");

	return 1;
}

//
// lru
//

lru_object* lru_object_create(int lru_size, int hash_table_size)
{
	lru_object *lru = malloc(sizeof(lru_object));
	if (lru == NULL)
		goto error;

	lru_list_init(&lru->lru);
	lru->max_size = (lru_size >= 5 ? lru_size : 5);
	lru->count = 0;

	lru->ht = htable_create(hash_table_size);
	if (lru->ht == NULL)
		goto error;

	goto out;

error:

	if (lru != NULL)
		free(lru);

	return NULL;

out:

	return lru;
}

int lru_object_destroy(lru_object *lru)
{
	if (lru == NULL)
		return -EINVAL;

	printf("============== lru destroy [max_size:%d] [count:%d] =============\n", lru->max_size, lru->count);

	int i = 0;
	htable_node *iter = lru->lru.lru_list_node.next;
	while (iter != &lru->lru)
	{
		printf("[%d: %d] ", ++i, iter->key);

		htable_node *next = iter->lru_list_node.next;

		htable_delete(lru->ht, iter->key);
		lru_list_delete(iter);

		free(iter);

		iter = next;
	}

	htable_destroy(lru->ht);
	free(lru);

	printf("\n\n");

	return 1;
}

htable_node* lru_object_get(lru_object *lru, int key)
{
	if (lru == NULL)
		return NULL;

	return htable_find(lru->ht, key);
}

htable_node* lru_object_access(lru_object *lru, int key)
{
	if (lru == NULL)
		return NULL;

	htable_node *ht_node = lru_object_get(lru, key);
	if (ht_node != NULL)
	{
		lru_list_delete(ht_node);
		lru_list_insert(lru->lru.lru_list_node.prev, ht_node);

		printf("lru cache hit[key:%d]\n", ht_node->key);
	}
	else
	{
		ht_node = malloc(sizeof(htable_node));
		if (ht_node == NULL)
			goto error;

		memset(ht_node, 0, sizeof(htable_node));
		ht_node->key = key;

		if (lru->count == lru->max_size)
		{
			int success = lru_object_eliminate(lru);
			if (success)
			{
				printf("lru eliminate success[max_size:%d][count:%d]\n", lru->max_size, lru->count);
			}
			else
			{
				printf("lru eliminate fail.\n");
				goto error;
			}
		}

		lru_list_insert(lru->lru.lru_list_node.prev, ht_node);
		htable_add(lru->ht, ht_node);
		lru->count++;

		printf("lru add new node[key:%d]\n", ht_node->key);
	}

	goto out;

error:

	if (ht_node != NULL)
		free(ht_node);

	return NULL;

out:

	return ht_node;
}

int lru_object_delete(lru_object *lru, htable_node *ht_node)
{
	if (lru == NULL || ht_node == NULL)
		return -EINVAL;

	htable_delete(lru->ht, ht_node->key);
	lru_list_delete(ht_node);
	lru->count--;

	free(ht_node);

	return 0;
}

int lru_object_eliminate(lru_object *lru)
{
	if (lru == NULL || lru->count == 0)
		return 0;

	htable_node *ht_node = lru->lru.lru_list_node.next;
	
	if (lru_object_delete(lru, ht_node) == 0)
		return 1;
	
	return 0;
}

int lru_object_traverse(lru_object *lru)
{
	if (lru == NULL)
		return -EINVAL;

	printf("============== lru list [max_size:%d] [count:%d] =============\n", lru->max_size, lru->count);

	lru_list_traverse(&lru->lru);
	
	printf("\n\n");

	return 1;
}

int main()
{
#define ARRAY_SIZE	10
	int a[ARRAY_SIZE] = { 5, 15, 55, 33, 22, 44, 55, 22, 88, 25 };

	lru_object *lru = lru_object_create(5, 10);
	if (lru == NULL)
	{
		printf("lru create fail.\n");
		exit(-1);
	}

	for (int i = 0; i < ARRAY_SIZE; i++)
	{
		printf("%d:\t[key:%d]\t", i, a[i]);

		lru_object_access(lru, a[i]);

		lru_object_traverse(lru);

		htable_traverse(lru->ht);
	}

	htable *htable = htable_create(10);
	if (htable == NULL)
	{
		printf("htable create fail.\n");
		exit(-1);
	}

	lru_object_destroy(lru);

	return 0;
}