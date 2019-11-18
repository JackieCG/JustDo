#include "def.h"
#include "08_queue.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

//
// array queue
//

#define DEFAULT_ARRAY_QUEUE_SIZE	5

int init_array_queue(struct array_queue *q, int size)
{
	if (q == NULL)
		return -EINVAL;

	if (size <= 0)
		size = DEFAULT_ARRAY_QUEUE_SIZE;

	q->data = malloc(sizeof(int) * size);
	if (q->data == NULL)
		return -ENOMEM;

	q->size = size;
	q->cnt = 0;

	return 0;
}

int fin_array_queue(struct array_queue *q)
{
	if (q == NULL)
		return -EINVAL;

	if (q->data != NULL)
		free(q->data);

	q->data = NULL;
	q->size = q->cnt = 0;

	return 0;
}

int array_enqueue(struct array_queue *q, int val)
{
	if (q == NULL)
		return -EINVAL;

	if (q->cnt == q->size)
	{
		int new_size = q->size * 2;
		int *new_data = realloc(q->data, sizeof(int) * new_size);
		if (new_data == NULL)
			return -ENOMEM;

		q->data = new_data;
		q->size = new_size;
	}

	q->data[q->cnt++] = val;

	return 0;
}

int array_dequeue(struct array_queue *q, int *val)
{
	if (q == NULL || val == NULL)
		return -EINVAL;

	if (q->cnt == 0)
		return -ENOENT;

	*val = q->data[0];
	memmove(q->data, q->data + 1, sizeof(int) * (q->cnt - 1));
	q->cnt--;

	return 0;
}

void dump_array_queue(struct array_queue *q)
{
	printf("dump_array_queue:\n");
	for (int i = 0; i < q->cnt; i++)
	{
		printf("%d: %d\n", i + 1, q->data[i]);
	}
}

int array_enqueue2(struct array_queue *q, int val)
{
	int ret = array_enqueue(q, val);

	if (ret == 0)
		printf("ENQUEUE:\t[val:%d]\n", val);
	else
		printf("ENQUEUE ERROR: %d\n", ret);

	return ret;
}

int array_dequeue2(struct array_queue *q, int *val)
{
	int ret = array_dequeue(q, val);

	if (ret == 0)
		printf("DEQUEUE:\t[val:%d]\n", *val);
	else
		printf("DEQUEUE ERROR: %d\n", ret);

	return ret;
}

//
// linked queue
//

int init_link_queue(struct link_queue *q)
{
	if (q == NULL)
		return -EINVAL;

	q->head = q->tail = NULL;

	return 0;
}

int fin_link_queue(struct link_queue *q)
{
	if (q == NULL)
		return -EINVAL;

	struct link_node *cur = q->head;
	while (cur != NULL)
	{
		struct link_node *next = cur->next;
		free(cur);
		cur = next;
	}

	q->head = q->tail = NULL;

	return 0;
}

int link_enqueue(struct link_queue *q, int val)
{
	if (q == NULL)
		return -EINVAL;

	struct link_node *new = malloc(sizeof(struct link_node));
	if (new == NULL)
	{
		return -ENOMEM;
	}

	new->data = val;
	new->next = NULL;

	if (q->head == NULL)
	{
		q->head = q->tail = new;
	}
	else
	{
		q->tail->next = new;
		q->tail = new;
	}

	return 0;
}

int  link_dequeue(struct link_queue *q, int *val)
{
	if (q == NULL || val == NULL)
		return -EINVAL;

	if (q->head == NULL)
		return -EINVAL;

	struct link_node *elem = q->head;
	if (q->head == q->tail)
	{
		q->head = q->tail = NULL;
	}
	else
	{
		q->head = q->head->next;
	}

	*val = elem->data;

	free(elem);

	return 0;
}

void dump_link_queue(struct link_queue *q)
{
	int i;
	struct link_node *cur;

	printf("dump_link_queue:\n");

	for (i = 0, cur = q->head; cur != NULL; i++, cur = cur->next)
	{
		printf("%d: %d\n", i + 1, cur->data);
	}
}

int link_enqueue2(struct link_queue *q, int val)
{
	int ret = link_enqueue(q, val);

	if (ret == 0)
		printf("ENQUEUE:\t[val:%d]\n", val);
	else
		printf("ENQUEUE ERROR: %d\n", ret);

	return ret;
}

int link_dequeue2(struct link_queue *q, int *val)
{
	int ret = link_dequeue(q, val);

	if (ret == 0)
		printf("DEQUEUE:\t[val:%d]\n", *val);
	else
		printf("DEQUEUE ERROR: %d\n", ret);

	return ret;
}

//
// circular queue
//

#define DEFAULT_CIRCULAR_QUEUE_SIZE	3

int init_circular_queue(struct circular_queue *q, int size)
{
	if (q == NULL)
		return -EINVAL;

	if (size < DEFAULT_CIRCULAR_QUEUE_SIZE)
		size = DEFAULT_CIRCULAR_QUEUE_SIZE;

	q->data = malloc(sizeof(int) * size);
	if (q->data == NULL)
		return -ENOMEM;

	q->size = size;
	q->start = q->end = 0;

	return 0;
}

int fin_circular_queue(struct circular_queue *q)
{
	if (q == NULL)
		return - EINVAL;

	if (q->data != NULL)
	{
		free(q->data);
		q->data = NULL;
	}

	q->size = q->start = q->end = 0;

	return 0;
}

int circular_enqueue(struct circular_queue *q, int val)
{
	if (q == NULL || q->data == NULL)
		return -EINVAL;

	// is full
	if ((q->end + 1) % q->size == q->start)
		return -ENOBUFS;

	q->data[q->end] = val;
	q->end = (q->end + 1) % q->size;

	return 0;
}

int circular_dequeue(struct circular_queue *q, int *val)
{
	if (q == NULL || q->data == NULL || val == NULL)
		return -EINVAL;

	// is empty
	if (q->start == q->end)
		return -ENODATA;

	*val = q->data[q->start];
	q->start = (q->start + 1) % q->size;

	return 0;
}

void dump_circular_queue(struct circular_queue *q)
{
	printf("dump_circular_queue:\n");

	if (q->data == NULL)
		return;

	int i, iter;
	for (iter = q->start, i = 1; iter != q->end; iter = (iter + 1) % q->size, i++)
	{
		printf("%d[%d]: %d\n", i, iter, q->data[iter]);
	}
}

int circular_enqueue2(struct circular_queue *q, int val)
{
	int ret = circular_enqueue(q, val);

	if (ret == 0)
		printf("ENQUEUE:\t[val:%d]\n", val);
	else
		printf("ENQUEUE ERROR: %d\n", ret);

	return ret;
}

int circular_dequeue2(struct circular_queue *q, int *val)
{
	int ret = circular_dequeue(q, val);

	if (ret == 0)
		printf("DEQUEUE:\t[val:%d]\n", *val);
	else
		printf("DEQUEUE ERROR: %d\n", ret);

	return ret;
}

/*
int main()
{
	//
	// array queue
	//

	//struct array_queue q = { 0 };
	//int val;
	//
	//init_array_queue(&q, 2);

	//array_enqueue2(&q, 1);
	//array_enqueue2(&q, 2);
	//array_enqueue2(&q, 3);
	//array_dequeue2(&q, &val);
	//array_enqueue2(&q, 4);
	//array_enqueue2(&q, 5);
	//array_dequeue2(&q, &val);
	//array_dequeue2(&q, &val);
	//array_dequeue2(&q, &val);

	//dump_array_queue(&q);

	//array_dequeue2(&q, &val);
	//array_dequeue2(&q, &val);
	//array_dequeue2(&q, &val);
	//array_enqueue2(&q, 6);
	//array_enqueue2(&q, 7);
	//array_dequeue2(&q, &val);
	//array_dequeue2(&q, &val);
	//array_dequeue2(&q, &val);
	//array_enqueue2(&q, 8);
	//array_enqueue2(&q, 9);

	//dump_array_queue(&q);

	//fin_array_queue(&q);

	//
	// linked queue
	//

	//struct link_queue q = { 0 };
	//int val;

	//init_link_queue(&q);

	//link_enqueue2(&q, 1);
	//link_enqueue2(&q, 2);
	//link_enqueue2(&q, 3);
	//link_dequeue2(&q, &val);
	//link_enqueue2(&q, 4);
	//link_enqueue2(&q, 5);
	//link_dequeue2(&q, &val);
	//link_dequeue2(&q, &val);
	//link_dequeue2(&q, &val);

	//dump_link_queue(&q);

	//link_dequeue2(&q, &val);
	//link_dequeue2(&q, &val);
	//link_dequeue2(&q, &val);
	//link_enqueue2(&q, 6);
	//link_enqueue2(&q, 7);
	//link_dequeue2(&q, &val);
	//link_dequeue2(&q, &val);
	//link_dequeue2(&q, &val);
	//link_enqueue2(&q, 8);
	//link_enqueue2(&q, 9);

	//dump_link_queue(&q);

	//fin_link_queue(&q);

	//
	// circular queue
	//

	struct circular_queue q = { 0 };
	int val;

	init_circular_queue(&q, 2);

	circular_enqueue2(&q, 1);
	circular_enqueue2(&q, 2);
	circular_enqueue2(&q, 3);
	circular_dequeue2(&q, &val);
	circular_enqueue2(&q, 4);
	circular_enqueue2(&q, 5);
	circular_dequeue2(&q, &val);
	circular_dequeue2(&q, &val);
	circular_dequeue2(&q, &val);

	dump_circular_queue(&q);

	circular_dequeue2(&q, &val);
	circular_dequeue2(&q, &val);
	circular_dequeue2(&q, &val);
	circular_enqueue2(&q, 6);
	circular_enqueue2(&q, 7);
	circular_dequeue2(&q, &val);
	circular_dequeue2(&q, &val);
	circular_dequeue2(&q, &val);
	circular_enqueue2(&q, 8);
	circular_enqueue2(&q, 9);

	dump_circular_queue(&q);

	fin_circular_queue(&q);

	return 0;
}
*/