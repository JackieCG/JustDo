#ifndef _LIST_H
#define _LIST_H

//
////////////////////////singly linked list/////////////////////
//

struct singly_link_list {
	int data;
	struct singly_link_list *next;
};

int singly_link_list_add(struct singly_link_list *head, int data);
int singly_link_list_del(struct singly_link_list *head, int data);
void singly_link_list_insert(struct singly_link_list *p_cur, struct singly_link_list *p_elem);
void singly_link_list_delete(struct singly_link_list *prev, struct singly_link_list *cur);
int singly_link_list_traverse(struct singly_link_list *head);

//
////////////////////////doubly linked list/////////////////////
//

struct doubly_link_list {
	int data;
	struct doubly_link_list *prev;
	struct doubly_link_list *next;
};

int doubly_link_list_add(struct doubly_link_list *head, int data);
int doubly_link_list_del(struct doubly_link_list *head, int data);
void doubly_link_list_insert(struct doubly_link_list *p_cur, struct doubly_link_list *p_elem);
void doubly_link_list_delete(struct doubly_link_list *cur);
int doubly_link_list_traverse(struct doubly_link_list *head);

//
////////////////////////circular linked list/////////////////////
//

void circular_link_list_init(struct doubly_link_list *head);
int circular_link_list_add(struct doubly_link_list *head, int data);
int circular_link_list_del(struct doubly_link_list *head, int data);
void circular_link_list_insert(struct doubly_link_list *p_cur, struct doubly_link_list *p_elem);
void circular_link_list_delete(struct doubly_link_list *cur);
int circular_link_list_traverse(struct doubly_link_list *head);

#endif