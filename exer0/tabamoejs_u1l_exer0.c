/* Tabamo, Euan Jed S. - CMSC 123 U-1L
** Exercise 0 - Diagnostic Exercise
** Date Created: August 20, 2024
*/

#include <stdio.h>
#include <stdlib.h>

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
    NODE *ptr = (NODE *)malloc(sizeof(NODE));
    if (ptr == NULL)
    {
        printf("ERROR: Memory allocation has failed.\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        ptr->value = data;
        ptr->next = NULL;
    }
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
    if (*head == NULL)
    {
        printf("ERROR: Linked List is empty.\n");
        return -1; // Return a -1 for an 'absence' of value in this case.
    }

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
    if (current == NULL)
    {
        printf("[]\n");
        return;
    }
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
    // Insertion
    insert(&head, 5);
    insert(&head, 4);
    insert(&head, 3);
    insert(&head, 2);
    insert(&head, 1);
    printList(head);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n"));

    // Deletion
    delete (&head);
    printList(head);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n"));
    delete (&head);
    printList(head);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n"));
    delete (&head);
    printList(head);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n"));
    delete (&head);
    printList(head);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n"));
    delete (&head);
    printList(head);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n"));

    // Deletion on an empty list
    delete (&head);
    printList(head);

    // isEmpty() on a filled list and an empty list
    insert(&head, 100);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n"));
    delete (&head);
    printf((isEmpty(head) ? "The list is empty.\n" : "The list is not empty.\n"));

    // Free memory
    while (head != NULL)
    {
        NODE *temp = head;
        head = head->next;
        free(temp);
    }

    return 0;
}