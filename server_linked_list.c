#include <stdio.h>
#include <stdlib.h>
#include "server.h"



list_n create_list() {
    node_n * head = NULL;
    list_n first;
    head = malloc(sizeof(*head));
    if (head == NULL) {
        printf("Malloc failed");
    }

    head->name = 1;
    head->next = NULL;
    first.head = head;
    return first;
}

void add_to_front(list_n * first, char* name) {
    node_n * new_node;
    new_node = malloc(sizeof(node_n));
    new_node->name = malloc(sizeof(name));
    new_node->name = name;
    new_node->next = first->head;
    first->head = new_node;
}

void add_to_end(list_n first, char* name) {
    node_n * current = first.head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = malloc(sizeof(node_n));
    current->next->name = name;
    current->next->next = NULL;

}

int pop(list_n *first) {

    int ret = -1;
    node_n * next_node = NULL;

    if(first->head == NULL) {
        return -1;
    }

    next_node = (first->head)->next;
    ret = sizeof((first->head)->name);
    free(first->head);
    first->head = next_node;

    return ret;
}

int remove_by_index(list_n *first, int val) {
    int i = 0, ret = -1;
    node_n * current = first->head;
    node_n * temp = NULL;

    if(val == 0) {
        return pop(first);
    }

    for(i = 0; i < val - 1; i++) {
        if (current->next == NULL) {
            return -1;
        }
        current = current->next; 
    }

    if (current->next == NULL) {
        return -1;
    }

    temp = current->next;
    ret = temp->name;
    current->next = temp->next;
    free(temp);
}

void print_list(list_n first) {
    node_n * current = first.head;

    while (current != NULL) {
        printf("%s\n", current->name);
        current = current->next;
    }
}

void free_list(list_n* list)
{
    node_n* tmp;

   while (list->head != NULL)
    {
       tmp = list->head;
       list->head = list->head->next;
       free(tmp);
       tmp = NULL;
    }

}