#include "def.h"
#include "07_stack.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>

//
// array stack
//

int init_stack(struct stack *s, int size)
{
	if (s == NULL || size <= 0)
		return -EINVAL;

	s->data = malloc(sizeof(int) * size);
	if (s->data == NULL)
		return -ENOMEM;

	s->size = size;
	s->cnt = 0;

	return 0;
}

void fin_stack(struct stack *s)
{
	if (s != NULL && s->data != NULL)
	{
		free(s->data);
		s->data = NULL;
		s->size = 0;
		s->cnt = 0;
	}
}

int push(struct stack *s, int val)
{
	if (s == NULL)
		return -EINVAL;

	if (s->cnt == s->size)
	{
		s->size = s->size * 2;
		
		int *new_buf = realloc(s->data, sizeof(int) * s->size);
		if (new_buf == NULL)
			return -ENOMEM;

		s->data = new_buf;
	}

	s->data[s->cnt++] = val;

	return 0;
}

int pop(struct stack *s, int *val)
{
	if (s == NULL || val == NULL)
		return -EINVAL;

	if (s->cnt == 0)
		return -1;

	*val = s->data[--s->cnt];

	return 0;
}

int push2(struct stack *s, int val)
{
	int ret = push(s, val);

	if (ret == 0)
		printf("PUSH:\t[val:%d]\n", val);
	else
		printf("PUSH ERROR: %d\n", ret);

	return ret;
}

int pop2(struct stack *s, int *val)
{
	int ret = pop(s, val);
	
	if (ret == 0)
		printf("POP:\t[val:%d]\n", *val);
	else
		printf("POP ERROR: %d\n", ret);

	return ret;
}

//
// linked list stack
//

int init_link_stack(struct link_stack *ls)
{
	if (ls == NULL)
		return -EINVAL;

	ls->head.next = NULL;
	ls->cnt = 0;

	return 0;
}

int fin_link_stack(struct link_stack *ls)
{
	if (ls == NULL)
		return -EINVAL;

	struct link_stack_node *cur, *next;
	for (cur = ls->head.next; cur != NULL; cur = next)
	{
		next = cur->next;
		
		printf("FIN: %d\n", cur->val);

		free(cur);
	}

	ls->head.next = NULL;
	ls->cnt = 0;
	
	return 0;
}

int link_stack_push(struct link_stack *ls, int val)
{
	if (ls == NULL)
		return -EINVAL;

	struct link_stack_node *new = malloc(sizeof(struct link_stack_node));
	if (new == NULL)
		return -ENOMEM;

	new->val = val;
	new->next = ls->head.next;
	ls->head.next = new;

	return 0;
}

int link_stack_pop(struct link_stack *ls, int *val)
{
	if (ls == NULL || val == NULL)
		return -EINVAL;

	if (ls->head.next == NULL)
		return -1;

	struct link_stack_node *node = ls->head.next;
	ls->head.next = node->next;

	*val = node->val;
	free(node);

	return 0;
}

int push3(struct link_stack *ls, int val)
{
	int ret = link_stack_push(ls, val);

	if (ret == 0)
		printf("PUSH:\t[val:%d]\n", val);
	else
		printf("PUSH ERROR: %d\n", ret);

	return ret;
}

int pop3(struct link_stack *ls, int *val)
{
	int ret = link_stack_pop(ls, val);

	if (ret == 0)
		printf("POP:\t[val:%d]\n", *val);
	else
		printf("POP ERROR: %d\n", ret);

	return ret;
}

/*
int main()
{
	//
	// browser forward & backward
	//

#define BUF_SIZE	256

	struct link_stack lsfor = { 0 };
	struct link_stack lsback = { 0 };

	struct link_stack *plsfor = &lsfor;
	struct link_stack *plsback = &lsback;

	char pbuf[BUF_SIZE] = { 0 };
	int init_flag = 0;
	int cur = 0;
	int val = 0;

	init_link_stack(plsfor);
	init_link_stack(plsback);

	while (1)
	{
		printf("Please input: ");

		char *instr = fgets(pbuf, BUF_SIZE, stdin);
		if (instr == NULL)
		{
			perror("fgets fail");
			break;
		}

		if (strncmp(instr, "quit", 4) == 0)
		{
			printf("quiting...\n");
			break;
		}
		else if (strncmp(instr, "f", 1) == 0)
		{
			int ret = pop3(plsfor, &val);
			if (ret == 0)
			{
				push3(plsback, cur);
				cur = val;
			}
		}
		else if (strncmp(instr, "b", 1) == 0)
		{
			int ret = pop3(plsback, &val);
			if (ret == 0)
			{
				push3(plsfor, cur);
				cur = val;
			}
		}
		else
		{
			val = atoi(instr);

			if (init_flag)
			{
				fin_link_stack(plsfor);
				push3(plsback, cur);
				cur = val;
			}
			else
			{
				cur = val;
				init_flag = 1;
			}
		}
	}

	fin_link_stack(plsfor);
	fin_link_stack(plsback);

	//
	// array stack
	//

	//struct stack s = { 0 };
	//int val;
	//
	//init_stack(&s, 2);

	//push2(&s, 1);
	//push2(&s, 2);
	//push2(&s, 3);
	//pop2(&s, &val);
	//push2(&s, 4);
	//push2(&s, 5);
	//pop2(&s, &val);
	//pop2(&s, &val);
	//pop2(&s, &val);
	//pop2(&s, &val);
	//pop2(&s, &val);
	//pop2(&s, &val);
	//push2(&s, 6);
	//push2(&s, 7);
	//pop2(&s, &val);
	//pop2(&s, &val);

	//fin_stack(&s);

	//
	// linked list stack
	//

	//struct link_stack s = { 0 };
	//int val;
	//
	//init_link_stack(&s);

	//push3(&s, 1);
	//push3(&s, 2);
	//push3(&s, 3);
	//pop3(&s, &val);
	//push3(&s, 4);
	//push3(&s, 5);
	//pop3(&s, &val);
	//pop3(&s, &val);
	//pop3(&s, &val);
	//pop3(&s, &val);
	//pop3(&s, &val);
	//pop3(&s, &val);
	//push3(&s, 6);
	//push3(&s, 7);
	//pop3(&s, &val);
	//pop3(&s, &val);
	//pop3(&s, &val);
	//push3(&s, 8);
	//push3(&s, 9);

	//fin_link_stack(&s);

	return 0;
}
*/