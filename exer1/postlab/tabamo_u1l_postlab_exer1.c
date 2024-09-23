#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void destroyQueue(LIST *L);

int main()
{
    LIST *bananaQueue = createQueue();

    enqueue(bananaQueue, createNode(1));
    enqueue(bananaQueue, createNode(2));
    enqueue(bananaQueue, createNode(3));
    enqueue(bananaQueue, createNode(4));
    enqueue(bananaQueue, createNode(5));
    printQueue(bananaQueue);

    printf("Removed %d\n", dequeue(bananaQueue));
    printf("Removed %d\n", dequeue(bananaQueue));
    printQueue(bananaQueue);

    printf(isEmpty(bananaQueue) ? "The queue is empty.\n" : "The queue is not empty.\n");

    while (!isEmpty(bananaQueue))
    {
        printf("Removed %d\n", dequeue(bananaQueue));
    }

    printQueue(bananaQueue);
    printf(isEmpty(bananaQueue) ? "The queue is empty.\n" : "The queue is not empty.\n");

    enqueue(bananaQueue, createNode(100));
    enqueue(bananaQueue, createNode(99));
    enqueue(bananaQueue, createNode(98));
    enqueue(bananaQueue, createNode(97));
    enqueue(bananaQueue, createNode(96));
    printQueue(bananaQueue);
    printf(isEmpty(bananaQueue) ? "The queue is empty.\n" : "The queue is not empty.\n");

    destroyQueue(bananaQueue);
}

void printQueue(LIST *L)
{
    // If the List is empty, print "*empty*"
    if (L->head == NULL)
    {
        printf("*empty*\n");
    }
    // Otherwise, print the contents of the list
    else
    {
        NODE *curr = L->head;
        while (curr != NULL)
        {
            printf("%d ", curr->value);
            curr = curr->next;
        }
        printf("\n");
    }
}

NODE *createNode(int data)
{
    // Allocate memory for a NODE
    NODE *new = (NODE *)malloc(sizeof(NODE));

    // NULL-checking
    if (new == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    // Initialize new NODE
    new->value = data;
    new->next = NULL;

    return new;
}

LIST *createQueue()
{
    // Allocate memory for a LIST
    LIST *new = (LIST *)malloc(sizeof(LIST));

    // NULL-checking
    if (new == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    // Initialize new LIST
    new->head = NULL;
    new->tail = NULL;

    return new;
}

int isEmpty(LIST *L)
{
    // If the head is NULL, then the LIST is empty
    return L->head == NULL;
}

void enqueue(LIST *L, NODE *node)
{
    // Case 1: The list is empty
    if (isEmpty(L))
    {
        L->head = L->tail = node;
    }
    // Case 2: The list is not empty
    else
    {
        L->tail->next = node;
        L->tail = node;
    }
}

int dequeue(LIST *L)
{
    // Case 1: The list is empty
    if (isEmpty(L))
    {
        printf("Error: Queue underflow\n");
        return -1; // Should probably be changed to a better sentinel value
    }
    // Case 2: The list is not empty
    else
    {
        int value = L->head->value;
        NODE *temp = L->head;
        L->head = L->head->next;

        if (L->head == NULL)
        {
            L->tail = NULL;
        }

        free(temp);
        return value;
    }
}

/*
** destroyQueue()
** requirements: any queue
** results:
    destroys a queue, freeing all memory allocated for the queue
*/
void destroyQueue(LIST *L)
{
    if (L == NULL)
    {
        return;
    }
    while (!isEmpty(L))
    {
        dequeue(L);
    }
    free(L);
}