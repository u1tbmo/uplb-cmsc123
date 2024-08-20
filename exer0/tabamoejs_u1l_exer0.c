/* Tabamo, Euan Jed S. - CMSC 123 U-1L
** Exercise 0 - Diagnostic Exercise
** Date Created: August 20, 2024
*/

#include <stdio.h>
#include <stdlib.h>

// Node Structure Definition
typedef struct node_tag
{
    int value;
    struct node_tag *next;
} NODE;

/*
** createNode()
** requirements: an integer data
** results:
    creates an empty node with value `data`
    initializes fields of the structure
    returns the created node
*/
NODE *createNode(int data)
{
    // Allocate memory for a node
    NODE *ptr = (NODE *)malloc(sizeof(NODE));
    // If memory allocation failed, exit the program with EXIT_FAILURE status
    if (ptr == NULL)
    {
        printf("ERROR: Memory allocation has failed.\n");
        exit(EXIT_FAILURE);
    }
    // Otherwise, initialize the fields of the node
    else
    {
        ptr->value = data;
        ptr->next = NULL;
    }
    // Return the created node
    return ptr;
}

/*
** isEmpty()
** results:
    returns 1 if the list is empty
    otherwise return 0
*/
int isEmpty(NODE *head)
{
    // If the head is NULL, the list is empty
    return head == NULL;
}

/*
** insert()
** requirements: the address of the head pointer and the value to be inserted
** results:
    inserts the newly created `node` at the `head` of the list
*/
void insert(NODE **head, int value)
{
    // Create a new node, then prepend it to the given list
    NODE *newNode = createNode(value);
    newNode->next = *head;
    *head = newNode;
}

/*
** delete()
** requirements: the address of the head pointer
** results:
    deletes the `head` node of the list
    returns the value of the deleted node
*/
int delete(NODE **head)
{
    // If the list is empty, print an error message and return -1 (sentinel value)
    if (*head == NULL)
    {
        printf("ERROR: Linked List is empty.\n");
        return -1; // Return a -1 for an 'absence' of value in this case.
    }

    // Otherwise, get the current head's value,
    // then update the head pointer to the next node
    // then free the memory of the previous head node
    // then return the value of the previous head
    NODE *temp = *head;
    int value = temp->value;
    *head = temp->next;
    free(temp);
    return value;
}

/*
** printList()
** requirements: head pointer
** results:
    prints the contents of the lists in a line
*/
void printList(NODE *head)
{
    NODE *current = head;
    // If the list is empty, print an empty list
    if (current == NULL)
    {
        printf("[]\n");
        return;
    }

    // Otherwise, print the list in the format: [<start>, ..., <end>]
    printf("[");
    while (current != NULL)
    {
        printf("%d", current->value);
        if (current->next != NULL)
        {
            printf(", ");
        }
        current = current->next;
    }
    printf("]\n");
}
int main()
{
    NODE *head = NULL;

    // TEST: Insertion
    insert(&head, 5);
    insert(&head, 4);
    insert(&head, 3);
    insert(&head, 2);
    insert(&head, 1);
    printList(head); // > [1, 2, 3, 4, 5]

    // TEST: Deletion
    delete (&head);
    printList(head); // > [2, 3, 4, 5]
    delete (&head);
    printList(head); // > [3, 4, 5]
    delete (&head);
    printList(head); // > [4, 5]
    delete (&head);
    printList(head); // > [5]
    delete (&head);
    printList(head); // > []

    // TEST: Deletion on an empty list
    delete (&head);  // > ERROR: Linked List is empty.
    printList(head); // > []

    // TEST: isEmpty() on a filled list and an empty list
    insert(&head, 100);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n")); // > The list is not empty.
    delete (&head);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n")); // > The list is empty.

    // Free memory
    while (head != NULL)
    {
        NODE *temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}