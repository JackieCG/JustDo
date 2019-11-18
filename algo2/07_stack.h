#ifndef _07_STACK
#define _07_STACK

//
// array stack
//

struct stack {
	int *data;
	int cnt;
	int size;
};

int init_stack(struct stack *s, int size);
void fin_stack(struct stack *s);
int push(struct stack *s, int val);
int pop(struct stack *s, int *val);

//
// linked list stack
//

struct link_stack_node {
	int val;
	struct link_stack_node *next;
};

struct link_stack {
	struct link_stack_node head;
	int cnt;
};

int init_link_stack(struct link_stack *ls);
int fin_link_stack(struct link_stack *ls);
int link_stack_push(struct link_stack *ls, int val);
int link_stack_pop(struct link_stack *ls, int *val);

#endif