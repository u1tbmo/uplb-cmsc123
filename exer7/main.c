/*
 * Name: Tabamo, Euan Jed S.
 * Lab Section:  U1L
 * Program Description: Implements a minheap with insert, delete
 * and heapsort functionality.
 */

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

// Prototypes for Additional Functions
void swap(int *array, int a, int b);
void percolateUp(HEAP *H);
void percolateDown(HEAP *H);

int left(int index) { return (2 * index); }

int right(int index) { return (2 * index + 1); }

int parent(int index) { return (index / 2); }

void printHeapHelper(HEAP *H, int index, int tabs) {
    if (index > H->size)
        return;
    else {
        printHeapHelper(H, right(index), tabs + 1);
        for (int i = 0; i < tabs; i++)
            printf("\t");
        printf("%d\n", H->heap[index]);
        printHeapHelper(H, left(index), tabs + 1);
    }
}

void printHeap(HEAP *H) {
    if (H != NULL && H->size > 0) {
        printHeapHelper(H, 1, 0);
    } else {
        printf("Empty heap!\n");
    }
}

HEAP *createHeap(int maxSize) {
    // Allocate memory for the heap
    HEAP *h = (HEAP *)malloc(sizeof(HEAP));
    if (h == NULL) {
        return NULL;
    }

    // Allocate memory for the array
    h->heap = (int *)malloc(maxSize * sizeof(int *) + 1);
    if (h->heap == NULL) {
        free(h);
        return NULL;
    }

    // Initialize all nodes to 0
    for (int i = 0; i < maxSize; i++) {
        h->heap[i] = 0;
    }

    // Initialize heap attributes size and maxSize
    h->size = 0;
    h->maxSize = maxSize;

    // Return the heap;
}

// The heap is full if the size is the maximum
int isFull(HEAP *H) { return H->size == H->maxSize; }

// The heap is full if the size is 0
int isEmpty(HEAP *H) { return H->size == 0; }

void clear(HEAP *H) {
    // Set all values to zero
    for (int i = 0; i < H->maxSize; i++) {
        H->heap[i] = 0;
    }

    // Reset the size of the heap
    H->size = 0;
}

/**
 * @brief Swaps two elements in an integer array
 *
 * @param array the integer array where two elements are located
 * @param a the index of the first element
 * @param b the index of the second element
 */
void swap(int *array, int a, int b) {
    int temp = array[a];
    array[a] = array[b];
    array[b] = temp;
}

/**
 * @brief Performs percolate up operation(s) to maintain the heap-order property
 * of a minheap
 *
 * @param H the heap to perform the operation on
 */
void percolateUp(HEAP *H) {
    // Get the index of the last added node
    int index = H->size;

    // While the index is in range and the heap-order is still not fixed
    while (index >= 1 && H->heap[parent(index)] > H->heap[index]) {
        swap(H->heap, index, parent(index)); // Swap the nodes in the array
        index = parent(index); // Get the new index for the next iteration
    }
}

/**
 * @brief Performs percolate down operation(s) to maintain the heap-order
 * property of a minheap
 *
 * @param H the heap to perform the operation on
 */
void percolateDown(HEAP *H) {
    // Start from the root node index
    int index = 1;

    // While the index is in range
    while (index <= H->size) {
        // Initialize the child to swap
        int child = -1;

        // If the left index is out of range, then so is the right
        // The operation is done since the node has no more children
        if (left(index) > H->size) {
            break;
        }

        // If the left index is in range but the right index is out of range,
        // then the left child is the only possible child left
        else if (right(index) > H->size) {
            child = left(index);
        }

        // If both are in range, the child to swap is the minimum
        // of the left and right child
        else {
            child = (H->heap[left(index)] < H->heap[right(index)])
                        ? left(index)
                        : right(index);
        }

        // If the current node is larger than the child, swap
        if (H->heap[index] > H->heap[child]) {
            swap(H->heap, index, child);
            index = child; // Get the new index for the next operation
        }
        // Otherwise, the heap-order is already satisfied
        else {
            break;
        }
    }
}

void insert(HEAP *H, int key) {
    // Insertion is impossible on a full heap
    if (isFull(H)) {
        printf("Heap is full!\n");
        return;
    }

    H->heap[++H->size] = key; // Insert the node to the array
    percolateUp(H);           // Percolate up to ensure heap-order property
}

int deleteM(HEAP *H) {
    // Deletion is impossible on an empty heap
    if (isEmpty(H)) {
        printf("Heap is empty!\n");
        return -1;
    }

    int m = H->heap[1];          // Retrieve the min value
    swap(H->heap, 1, H->size--); // Swap the root and end node, decrement size
    percolateDown(H);            // Percolate down to ensure heap-order property

    return m;
}

int *heapSort(HEAP *H) {
    // Create a copy of the heap
    HEAP *copy = createHeap(H->maxSize);

    // Copy the heap values to the copy
    for (int i = 1; i <= H->size; i++) {
        copy->heap[i] = H->heap[i];
        copy->size++;
    }

    // Perform deletions to sort the heap
    for (int i = 0; i < H->size; i++) {
        deleteM(copy);
    }

    // Create an array to store the sorted values
    int *sorted = (int *)malloc((H->size + 1) * sizeof(int));

    // Copy the sorted values from the heap to the array
    for (int i = 1; i <= H->size; i++) {
        sorted[i] = copy->heap[i];
    }

    // Free the copy
    free(copy->heap);
    free(copy);

    // Return the sorted array
    return sorted;
}

int main() {

    char command;
    int key, result, type;
    int *sorted;

    HEAP *H = createHeap(30);

    while (1) {
        scanf(" %c", &command);

        switch (command) {
        case '+':
            scanf("%d", &key);
            printf("Inserting key: %d...\n", key);
            insert(H, key);
            break;
        case '-':
            printf("Removing root from tree...\n");
            result = deleteM(H); // result is unused
            break;
        case 'p':
            printf("Printing the heap (rotated +90 degrees)...\n");
            printHeap(H);
            printf("\n");
            break;
        case 'E':
            printf("Heap %s empty.\n", isEmpty(H) ? "is" : "is not");
            break;
        case 'F':
            printf("Heap %s full.\n", isFull(H) ? "is" : "is not");
            break;
        case 'C':
            printf("Removing all contents...\n");
            clear(H);
            break;
        case '~':
            printf("The sorted version of the heap:\n");
            sorted = heapSort(H);
            for(key=1; key <= H->size; key++)
                printf("%4d", sorted[key]);
            printf("\n");
            free(sorted);
            break;
        case 'Q':
            free(H->heap);
            free(H);
            return 0;
        default:
            printf("Unknown command: %c\n", command);
        }
    }

    return 0;
}
