/* Tabamo, Euan Jed S. - CMSC 123 U-1L
** Exercise 9 - Graphs
** Description: Implementation of graphs using adjacency matrices and traversal
*using DFS.
** Date Created: November 26, 2024
*/

#include "graph.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

// STACK FUNCTIONS

void printStack(STACK *L) {
    if (L == NULL) {
        printf("The stack is NULL.\n");
        return;
    }

    if (isEmpty(L)) {
        printf("The stack is empty!\n");
        return;
    }

    // Print each value starting from the head
    NODE *curr = L->head;
    while (curr != NULL) {
        printf("%d ", curr->value);
        curr = curr->next;
    }
    printf("\n");
}

NODE *createNode(int data) {
    // Allocate memory for the node
    NODE *newNode = (NODE *)malloc(sizeof(NODE));
    if (newNode == NULL) {
        return NULL;
    }

    // Initialize value of the node
    newNode->value = data;

    return newNode;
}

STACK *createStack() {
    // Allocate memory for the stack
    STACK *newStack = (STACK *)malloc(sizeof(STACK));
    if (newStack == NULL) {
        return NULL;
    }

    // Initialize pointer to NULl
    newStack->head = NULL;

    return newStack;
}

int isEmpty(STACK *L) { return L->head == NULL; }

void push(STACK *L, NODE *node) {
    // Insert when stack is empty
    if (isEmpty(L)) {
        L->head = node;
        return;
    }

    // Insert when stack is not empty
    node->next = L->head;
    L->head = node;
}

int pop(STACK *L) {
    // Stack underflow if empty
    if (isEmpty(L)) {
        printf("Stack underflow!\n");
    }

    // Delete the TOS
    NODE *temp = L->head;
    int value = temp->value;
    L->head = temp->next;
    free(temp);
    return value;
}

// GRAPH FUNCTIONS

GRAPH *createGraph(int vertices) {
    // Allocate memory for the graph
    GRAPH *newGraph = (GRAPH *)malloc(sizeof(GRAPH));
    if (newGraph == NULL) {
        return NULL;
    }

    // Allocate memory for the 2D matrix
    newGraph->matrix = (int **)malloc(vertices * sizeof(int *));
    for (int i = 0; i < vertices; i++) {
        newGraph->matrix[i] = (int *)malloc(vertices * sizeof(int));
        if (newGraph->matrix[i] == NULL) {
            // Free allocated memory thus far
            for (int j = 0; j < i; j++) {
                free(newGraph->matrix[i]);
            }
            free(newGraph->matrix);
            return NULL;
        }
    }

    // Initialize number of vertices and matrix
    newGraph->num_vertices = vertices;
    for (int i = 0; i < vertices; i++) {
        for (int j = 0; j < vertices; j++) {
            newGraph->matrix[i][j] = 0;
        }
    }

    return newGraph;
}

void insertEdge(GRAPH *G, int u, int v) {
    // Ensure the edge is valid for the graph.
    if (u < 0 || v < 0 || u > G->num_vertices || v > G->num_vertices) {
        return;
    }

    // Mark as an undirected edge
    // Indexes are already adjusted in main
    G->matrix[u][v] = 1;
    G->matrix[v][u] = 1;
}

int *createVisited(GRAPH *G) {
    // Allocate memory for an integer array
    int *visited = (int *)malloc(G->num_vertices * sizeof(int));
    if (visited == NULL) {
        return NULL;
    }

    // Initialize to 0s (unvisited)
    for (int i = 0; i < G->num_vertices; i++) {
        visited[i] = 0;
    }

    return visited;
}

void dfs(GRAPH *G, int start) {
    // Create a stack
    STACK *stack = createStack();

    // Create a visited array
    int *visited = createVisited(G);
    visited[start] = 1; // Visit the source node, index already adjusted in main

    // Push the source node to the stack
    push(stack, createNode(start));

    // Visit all nodes possible using dfs while the stack isEmpty
    while (!isEmpty(stack)) {
        // Visit the TOS (and print)
        int u = pop(stack);
        printf("%d ", u + 1);
        visited[u] = 1;

        // Push unvisited vertices adjacent to u to the TOS
        // We traverse the array in reverse since we want
        // the head to be the smallest index (LIFO)
        for (int v = G->num_vertices; v >= 0; v--) {
            if (G->matrix[u][v] == 1 && !visited[v]) {
                push(stack, createNode(v));
            }
        }
    }

    // Free allocated memory
    free(stack);
    free(visited);
}

void printMatrix(GRAPH *G) {
    // Print each element in the matix
    for (int i = 0; i < G->num_vertices; i++) {
        for (int j = 0; j < G->num_vertices; j++) {
            printf("%d\t", G->matrix[i][j]);
        }
        printf("\n");
    }
}

void freeMatrix(GRAPH *G) {
    if (G != NULL) {
        // Free the outer array
        if (G->matrix != NULL) {
            // Free the inner arrays
            for (int i = 0; i < G->num_vertices; i++) {
                free(G->matrix[i]);
                G->matrix[i] = NULL;
            }
            free(G->matrix);
            G->matrix = NULL;
        }
        G->matrix = NULL;
    }
}

int main() {
    char command;
    int vertices, u, v;

    printf("QUEEN NEVER CRY!\n");
    scanf("%d", &vertices);
    GRAPH *G = createGraph(vertices);

    while (1) {
        scanf(" %c", &command);

        switch (command) {
        case '+':
            scanf(" %d %d", &u, &v);
            insertEdge(
                G, u - 1,
                v - 1); // there's a -1 since we use 0-indexing in the arrays
            printf("Successfully inserted edge %d %d\n", u, v);
            break;
        case '#':
            printf("\nDFS: ");
            dfs(G, 0); // 0 is the initial value since we use 0-indexing (it
                       // still represents vertex 1)
            printf("\n");
            break;
        case 'p':
            printf("\nADJACENCY MATRIX: \n");
            printMatrix(G);
            break;
        case 'f':
            freeMatrix(G);
            break;
        case 'Q':
            freeMatrix(G);
            free(G);
            return 0;
        default:
            printf("Unknown command: %c\n", command);
        }
    }
}
