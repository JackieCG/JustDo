#include "def.h"
#include "06_list.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

//
////////////////////////singly linked list/////////////////////
//

int singly_link_list_add(struct singly_link_list *head, int data)
{
	if (head == NULL)
		return -EINVAL;

	struct singly_link_list *p_elem = (struct singly_link_list *)malloc(sizeof(struct singly_link_list));
	if (p_elem == NULL)
	{
		perror("malloc fail");
		return -ENOMEM;
	}
	
	p_elem->data = data;
	p_elem->next = NULL;

	struct singly_link_list *cur = head;
	while (cur->next != NULL && cur->next->data < p_elem->data)
		cur = cur->next;

	singly_link_list_insert(cur, p_elem);

	return 0;
}

int singly_link_list_del(struct singly_link_list *head, int data)
{
	if (head == NULL)
		return -EINVAL;

	struct singly_link_list *prev, *cur;
	prev = head;
	cur = head->next;
	while (cur != NULL && cur->data != data)
		cur = cur->next;

	if (cur != NULL)
	{
		singly_link_list_delete(prev, cur);

		free(cur);

		return 1;
	}

	return 0;
}

void singly_link_list_insert(struct singly_link_list *p_cur, struct singly_link_list *p_elem)
{
	p_elem->next = p_cur->next;
	p_cur->next = p_elem;
}

void singly_link_list_delete(struct singly_link_list *prev, struct singly_link_list *cur)
{
	prev->next = cur->next;
}

int singly_link_list_traverse(struct singly_link_list *head)
{
	if (head == NULL)
		return -EINVAL;

	int i = 0;
	struct singly_link_list *iter = head;
	while ((iter = iter->next) != NULL)
	{
		printf("[%d: %d] ", ++i, iter->data);
	}
	printf("\n");

	return 1;
}

//
////////////////////////doubly linked list/////////////////////
//

int doubly_link_list_add(struct doubly_link_list *head, int data)
{
	if (head == NULL)
		return -EINVAL;

	struct doubly_link_list *p_elem = (struct doubly_link_list *)malloc(sizeof(struct doubly_link_list));
	if (p_elem == NULL)
	{
		perror("malloc fail");
		return -ENOMEM;
	}

	p_elem->data = data;
	p_elem->prev = p_elem->next = NULL;

	struct doubly_link_list *cur = head;
	while (cur->next != NULL && cur->next->data < p_elem->data)
		cur = cur->next;

	doubly_link_list_insert(cur, p_elem);

	return 0;
}

int doubly_link_list_del(struct doubly_link_list *head, int data)
{
	if (head == NULL)
		return -EINVAL;

	struct doubly_link_list *cur = head->next;
	while (cur != NULL && cur->data != data)
		cur = cur->next;

	if (cur != NULL)
	{
		doubly_link_list_delete(cur);

		free(cur);

		return 1;
	}

	return 0;
}

void doubly_link_list_insert(struct doubly_link_list *p_cur, struct doubly_link_list *p_elem)
{
	p_elem->next = p_cur->next;
	p_cur->next = p_elem;
	p_elem->prev = p_cur;
	if (p_elem->next != NULL)
		p_elem->next->prev = p_elem;
}

void doubly_link_list_delete(struct doubly_link_list *cur)
{
	cur->prev->next = cur->next;
	if (cur->next != NULL)
		cur->next->prev = cur->prev;
}

int doubly_link_list_traverse(struct doubly_link_list *head)
{
	if (head == NULL)
		return -EINVAL;

	int i = 0;
	struct doubly_link_list *iter = head;
	while ((iter = iter->next) != NULL)
	{
		printf("[%d: %d] ", ++i, iter->data);
	}
	printf("\n");

	for (iter = head->next; iter != NULL && iter->next != NULL; iter = iter->next);
	if (iter != NULL)
	{
		printf("reveser:");
		for (; iter != head; iter = iter->prev)
		{
			printf("[%d: %d] ", i--, iter->data);
		}
		printf("\n");
	}

	return 1;
}

//
////////////////////////circular linked list/////////////////////
//

void circular_link_list_init(struct doubly_link_list *head)
{
	head->prev = head->next = head;
}

int circular_link_list_add(struct doubly_link_list *head, int data)
{
	if (head == NULL)
		return -EINVAL;

	struct doubly_link_list *p_elem = (struct doubly_link_list *)malloc(sizeof(struct doubly_link_list));
	if (p_elem == NULL)
	{
		perror("malloc fail");
		return -ENOMEM;
	}

	p_elem->data = data;
	p_elem->prev = p_elem->next = NULL;

	struct doubly_link_list *cur = head;
	while (cur->next != head && cur->next->data < p_elem->data)
		cur = cur->next;

	circular_link_list_insert(cur, p_elem);

	return 0;
}

int circular_link_list_del(struct doubly_link_list *head, int data)
{
	if (head == NULL)
		return -EINVAL;

	struct doubly_link_list *cur = head->next;
	while (cur != head && cur->data != data)
		cur = cur->next;

	if (cur != head)
	{
		circular_link_list_delete(cur);

		free(cur);

		return 1;
	}

	return 0;
}

void circular_link_list_insert(struct doubly_link_list *p_cur, struct doubly_link_list *p_elem)
{
	p_elem->next = p_cur->next;
	p_cur->next = p_elem;
	p_elem->prev = p_cur;
	p_elem->next->prev = p_elem;
}

void circular_link_list_delete(struct doubly_link_list *cur)
{
	cur->prev->next = cur->next;
	cur->next->prev = cur->prev;
}

int circular_link_list_traverse(struct doubly_link_list *head)
{
	if (head == NULL)
		return -EINVAL;

	int flag = 1;
	int i = 0;
	struct doubly_link_list *iter = head;
	while ((iter = iter->next) != head || (flag-- != 0 && (iter = iter->next) != head))
	{
		printf("[%d: %d] ", ++i, iter->data);
	}
	printf("\n");

	for (iter = head->next; iter != head && iter->next != head; iter = iter->next);
	if (iter != head)
	{
		printf("reveser:");
		for (flag = 1; iter != head || (flag-- != 0 && (iter = iter->prev) != head); iter = iter->prev)
		{
			printf("[%d: %d] ", i--, iter->data);
		}
		printf("\n");
	}

	return 1;
}

/*
int main()
{
	//
	////////////////////////singly linked list/////////////////////
	//

	//struct singly_link_list head = { 0 };

	//singly_link_list_add(&head, 6);
	//singly_link_list_add(&head, 2);
	//singly_link_list_add(&head, 4);
	//singly_link_list_add(&head, 8);
	//singly_link_list_add(&head, 12);
	//singly_link_list_add(&head, 10);

	//printf("singly_link_list(after add):\n");
	//singly_link_list_traverse(&head);

	//singly_link_list_del(&head, 6);
	//singly_link_list_del(&head, 2);
	//singly_link_list_del(&head, 4);
	//singly_link_list_del(&head, 8);
	//singly_link_list_del(&head, 12);
	//singly_link_list_del(&head, 10);

	//printf("singly_link_list(after del):\n");
	//singly_link_list_traverse(&head);

	//
	////////////////////////doubly linked list/////////////////////
	//

	//struct doubly_link_list head = { 0 };

	//doubly_link_list_add(&head, 6);
	//doubly_link_list_add(&head, 2);
	//doubly_link_list_add(&head, 4);
	//doubly_link_list_add(&head, 8);
	//doubly_link_list_add(&head, 12);
	//doubly_link_list_add(&head, 10);

	//printf("doubly_link_list(after add):\n");
	//doubly_link_list_traverse(&head);

	//doubly_link_list_del(&head, 6);
	//doubly_link_list_del(&head, 2);
	//doubly_link_list_del(&head, 4);
	//doubly_link_list_del(&head, 8);
	//doubly_link_list_del(&head, 12);
	//doubly_link_list_del(&head, 10);

	//printf("doubly_link_list(after del):\n");
	//doubly_link_list_traverse(&head);

	//
	////////////////////////circular linked list/////////////////////
	//

	struct doubly_link_list head = { 0 };
	circular_link_list_init(&head);

	circular_link_list_add(&head, 6);
	circular_link_list_add(&head, 2);
	circular_link_list_add(&head, 4);
	circular_link_list_add(&head, 8);
	circular_link_list_add(&head, 12);
	circular_link_list_add(&head, 10);

	printf("circular_link_list(after add):\n");
	circular_link_list_traverse(&head);

	circular_link_list_del(&head, 6);
	circular_link_list_del(&head, 2);
	circular_link_list_del(&head, 4);
	circular_link_list_del(&head, 8);
	circular_link_list_del(&head, 12);
	circular_link_list_del(&head, 10);

	printf("circular_link_list(after del):\n");
	circular_link_list_traverse(&head);

	return 0;
}
*/