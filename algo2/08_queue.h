#ifndef _08_QUEUE
#define _08_QUEUE

//
// array queue
//

struct array_queue {
	int *data;
	int cnt;
	int size;
};

int init_array_queue(struct array_queue *q, int size);
int fin_array_queue(struct array_queue *q);
int array_enqueue(struct array_queue *q, int val);
int array_dequeue(struct array_queue *q, int *val);
void dump_array_queue(struct array_queue *q);

//
// linked queue
//

struct link_node {
	int data;
	struct link_node *next;
};

struct link_queue {
	struct link_node *head;
	struct link_node *tail;
};

//
// circular queue
//

struct circular_queue {
	int *data;
	int size;
	int start;
	int end;
};

#endif