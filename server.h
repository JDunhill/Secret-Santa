#ifndef __LINKEDLISTS_H__
#define __LINKEDLISTS_H__



typedef struct node {
    char* name;
    struct node * next;
} node_n;

typedef struct list {
    struct node * head;
} list_n;

list_n create_list();

void add_to_front(list_n * first, char* name);

void add_to_end(list_n first, char* name);

int pop(list_n * first);

int remove_by_index(list_n * first, int val);

void print_list(list_n first);

void free_list(list_n* first);

#endif  