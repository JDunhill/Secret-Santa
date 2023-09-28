#ifndef __LINKEDLISTS_H__
#define __LINKEDLISTS_H__



typedef struct node {
    char* name;
    struct node * next;
} node_n;

void add_to_front(node_n ** head, char* name);

void add_to_end(node_n** head, char* name);

int pop(node_n ** head);

int remove_by_index(node_n ** head, int val);

void print_list(node_n* head);

void free_list(node_n* head);

void append(node_n** head, char* name);

int count_list(node_n* head);

#endif  