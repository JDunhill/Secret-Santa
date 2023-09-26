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
    printf("\nin add to end\n");

    if(current->name == NULL) {
        printf("a\n");
        current->name = malloc(sizeof(name) + 1);
        strlcpy(current->name, name, sizeof(name + 1));
        current->next = NULL;
    } else {
        printf("b\n");
        while (1) {
            printf("looping");
            current = current->next;
            if(current->next != NULL) {
                break;
            
            }
        }
        if ((current->next = malloc(sizeof(node_n))) == NULL) {
            printf("Malloc node failure\n");
        }
        printf("c\n");
        current->name = malloc(sizeof(name) + 1);
        strlcpy(current->name, name, sizeof(name + 1));
        current->next->next = NULL;
    
    }

}

void append(node_n** head, char* name)
{
    /* 1. allocate node */
    node_n *current = malloc(sizeof(node_n));
 
    node_n *last = *head; /* used in step 5*/
 
    /* 2. put in the data */
    current->name = malloc(sizeof(name) + 1);
    strlcpy(current->name, name, sizeof(name + 1));
    printf("\nCurrent name is: %s\n", current->name);
 
    /* 3. This new node is going to be the last node, so
    make next of it as NULL*/
    current->next = NULL;
 
    /* 4. If the Linked List is empty, then make the new
    * node as head */
    if (last->next == NULL) {
        *head = current;
        return;
    }
 
    /* 5. Else traverse till the last node */
    while (last->next != NULL)
        last = last->next;
 
    /* 6. Change the next of last node */
    last->next = current;
    return;
}

int pop(node_n **head) {

    int ret = -1;
    node_n * next_node = NULL;

    if(head == NULL) {
        return -1;
    }

    next_node = **head->next;
    ret = sizeof((*head)->name);
    free(head);
    head = next_node;

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
    printf("\nIn printlist\n");
    // while (current != NULL && current->name != NULL) {
    //     printf("Node: %s\n", current->name);
    //     current = current->next;
    // }
    printf("Node: %s\n", current->name);
    printf("Node: %s\n", current->next->name);

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