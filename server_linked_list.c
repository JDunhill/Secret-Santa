#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"




// list_n create_list() {
//     node_n * head = NULL;
//     list_n first;
//     head = malloc(sizeof(*head));
//     if (head == NULL) {
//         printf("Malloc failed");
//     }

//     head->name = NULL;
//     head->next = NULL;
//     first.head = head;
//     return first;
// }

void add_to_front(node_n** head, char* name) {
    node_n * new_node;
    new_node = malloc(sizeof(node_n));
    new_node->name = malloc(sizeof(name) + 1);

    if(new_node->name == NULL) {
        strlcpy(new_node->name, name, sizeof(name + 1));
        new_node->next = NULL;
    } else {
        strlcpy(new_node->name, name, sizeof(name + 1));
        new_node->next = *head;
        *head = new_node;
}
}


void add_to_end(node_n** head, char* name) {
    node_n * current = *head;

    if(current->name == NULL) {
        
        current->name = malloc(sizeof(name) + 1);
        strlcpy(current->name, name, sizeof(name + 1));
        current->next = NULL;
    } else {
        
        while (current->next != NULL) {
            current = current->next;
        }
        if ((current->next = malloc(sizeof(node_n))) == NULL) {
            printf("Malloc node failure\n");
        }
        
        current->next->name = malloc(sizeof(name) + 1);
        strlcpy(current->next->name, name, sizeof(name + 1));
        current->next->next = NULL;
    
    }
    

}


int pop(node_n **head) {

    int ret = -1;
    node_n * next_node = NULL;

    if(head == NULL) {
        return -1;
    }
    node_n * current = *head;
    next_node = current->next;
    ret = sizeof((*head)->name);
    free(head);
    *head = next_node;

    return ret;
}

int remove_by_index(node_n **head, int val) {
    int i = 0, ret = -1;
    node_n * current = *head;
    node_n * temp = NULL;

    if(val == 0) {
        return pop(head);
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
    ret = sizeof(temp->name);
    current->next = temp->next;
    free(temp);
    return 0;
}

void print_list(node_n* head) {
    node_n * current = head;
    
    while (current != NULL && current->name != NULL) {
        printf("Node: %s\n", current->name);
        current = current->next;
    }

    printf("\nFinished printing.\n");
   

}

int count_list(node_n* head){
    node_n * current = head;
    static int counter = 0;
    while (current->next != NULL && current->name != NULL) {
        printf("Node: %s\n", current->name);
        current = current->next;
        counter++;
    }
    
    return counter;

}


void free_list(node_n* head)
{
    node_n* tmp;

   while (head != NULL)
    {
       tmp = head;
       head = head->next;
       free(tmp);
       tmp = NULL;
    }

}