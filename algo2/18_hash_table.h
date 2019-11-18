#ifndef _18_HASH_TABLE
#define _18_HASH_TABLE

#define HTABLE	0
#define LRU		1

typedef struct _ht_list_node {
	//struct _htable_node *prev;
	struct _htable_node *next;
} ht_list_node;

typedef struct _lru_list_node {
	struct _htable_node *prev;
	struct _htable_node *next;
} lru_list_node;

typedef struct _htable_node {
	int key;

	ht_list_node ht_list_node;

	lru_list_node lru_list_node;
} htable_node;

typedef struct _htable {
	ht_list_node *hash_table;
	int size;

	//htable_node lru;
	//int count;
} htable;

typedef struct _lru_object {
	htable_node lru;
	int count;
	int max_size;

	htable *ht;
} lru_object;

void ht_list_insert(ht_list_node *p_cur, htable_node *node);
void ht_list_del(ht_list_node *prev, ht_list_node *cur);
int ht_list_delete(ht_list_node *head, int key);
htable_node* ht_list_find(ht_list_node *head, int key);
int ht_list_traverse(ht_list_node *head);

int htable_hash(htable *htable, int key);
htable* htable_create(int size);
int htable_destroy(htable *htable);
htable_node* htable_find(htable *htable, int key);
htable_node* htable_add_new(htable *htable, int key);
int htable_add(htable *htable, htable_node *ht_node);
int htable_delete(htable *htable, int key);
void htable_traverse(htable *htable);

void lru_list_init(htable_node *head);
void lru_list_insert(htable_node *p_cur, htable_node *p_elem);
void lru_list_delete(htable_node *cur);
int lru_list_traverse(htable_node *lru_head);

lru_object* lru_object_create(int lru_size, int hash_table_size);
int lru_object_destroy(lru_object *lru);
htable_node* lru_object_get(lru_object *lru, int key);
htable_node* lru_object_access(lru_object *lru, int key);
int lru_object_delete(lru_object *lru, htable_node *ht_node);
int lru_object_eliminate(lru_object *lru);
int lru_object_traverse(lru_object *lru);

#endif