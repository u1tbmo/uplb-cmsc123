/* Tabamo, Euan Jed S. - CMSC 123 U-1L
** Exercise 10 - Dijkstra's Algorithm
** Description: Implementation of dijkstra's algorithm for directed graphs.
** Date Created: December 3, 2024
*/
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

// Define the maximum integer limit as infinity
const int INFINITY = 2147483647;

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

void insertEdge(GRAPH *G, int u, int v, int w) {
    // Ensure the edge is valid for the graph.
    if (u < 0 || v < 0 || u > G->num_vertices - 1 || v > G->num_vertices - 1) {
        return;
    }

    // Mark as a weighted directed edge
    G->matrix[u][v] = w;
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

int *createDistance(GRAPH *G) {
    // Allocate memory for an integer array
    int *distance = (int *)malloc(G->num_vertices * sizeof(int));
    if (distance == NULL) {
        return NULL;
    }

    // Initialize to infinity or the max integer limit
    for (int i = 0; i < G->num_vertices; i++) {
        distance[i] = INFINITY;
    }

    return distance;
}

int *createParent(GRAPH *G) {
    // Allocate memory for an integer array
    int *parents = (int *)malloc(G->num_vertices * sizeof(int));
    if (parents == NULL) {
        return NULL;
    }

    // Initialize to -1 (no parent)
    for (int i = 0; i < G->num_vertices; i++) {
        parents[i] = -1;
    }

    return parents;
}

void dijkstra(GRAPH *G, int source, int target) {
    // Ensure the source and/or target are valid
    if (source < 0 || target < 0 || source > G->num_vertices - 1 ||
        target > G->num_vertices - 1) {
        printf("Error: source and/or target not in graph\n");
        return;
    }

    // Create arrays needed for the algorithm
    int *parent = createParent(G);
    int *visited = createVisited(G);
    int *distance = createDistance(G);

    // Set the source node to 0
    distance[source] = 0;

    // While there are still nodes to visit
    while (1) {
        // Obtain the unvisited node u with the minimum distance
        int u = -1;
        int minimumDistance = INFINITY;
        for (int i = 0; i < G->num_vertices; i++) {
            // Skip visited nodes
            if (visited[i]) {
                continue;
            }

            // Find the minimum distance
            if (distance[i] < minimumDistance) {
                u = i;
                minimumDistance = distance[i];
            }
        }

        // If minimumDistance == INFINITY, every node has been visited
        if (minimumDistance == INFINITY) {
            break;
        }

        // Visit the node
        visited[u] = 1;

        // Iterate over every unvisited adjacent vertex v in the graph
        for (int v = 0; v < G->num_vertices; v++) {
            // If weight == 0, then v is not adjacent to u
            if (G->matrix[u][v] == 0) {
                continue;
            }

            // Compute for the alternate distance
            int alt_dist = distance[u] + G->matrix[u][v];

            // If the alternate distance is shorter,
            // Update the parent and distance
            if (alt_dist < distance[v]) {
                parent[v] = u;
                distance[v] = alt_dist;
            }
        }
    }

    // DEBUGGING: PRINTS THE PARENT, VISITED, AND DISTANCE ARRAYS
    // printf("|  n |  p |  v |   d |\n");
    // for (int i = 0; i < G->num_vertices; i++) {
    //     if (distance[i] != INFINITY) {
    //         printf("| %2d | %2d | %2d | %3d |\n", i + 1, parent[i] + 1,
    //                visited[i], distance[i]);
    //     } else {
    //         printf("| %2d | %2d | %2d | INF |\n", i + 1, parent[i] + 1,
    //                visited[i]);
    //     }
    // }

    // If the distance of the target from the source is INFINITY
    // Then there is no path from the source to target.
    if (distance[target] == INFINITY) {
        // + 1 to adjust for input which is 0-indexed
        printf("PATH FROM %d to %d: IMPOSSIBLE!\n", source + 1, target + 1);
        printf("DISTANCE FROM %d to %d: INFINITY\n", source + 1, target + 1);
        return;
    }

    // Create an array to store the backwards traversal to be printed out
    int *traversal = (int *)malloc(sizeof(int) * G->num_vertices);
    for (int i = 0; i < G->num_vertices; i++) {
        // Set to a sentinel value to indicate as empty
        traversal[i] = -1;
    }

    // Set the target as the last node traversed
    traversal[0] = target + 1;

    // Traverse backwards
    // Insert traversed nodes in the traversal array
    int currentParent = parent[target];
    int traversalIdx = 0;
    while (currentParent != -1) {
        traversal[++traversalIdx] = currentParent + 1;
        currentParent = parent[currentParent];
    }

    // Print the traversed path
    printf("PATH FROM %d to %d: ", source + 1, target + 1);
    for (int i = G->num_vertices - 1; i >= 0; i--) {
        // Only print what is actually in the traversal
        if (traversal[i] != -1) {
            printf("%d ", traversal[i]);
        }
    }
    printf("\n");

    // Print distance
    printf("DISTANCE FROM %d to %d: %d\n", source + 1, target + 1,
           distance[target]);

    // Free the traversal array
    free(traversal);
}

void printMatrix(GRAPH *G) {
    // Print each element in the matix
    for (int i = 0; i < G->num_vertices; i++) {
        for (int j = 0; j < G->num_vertices; j++) {
            printf("%d\t", G->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
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
    int vertices, lines, u, v, w, source, target;

    scanf("%d", &vertices);
    GRAPH *G = createGraph(vertices);

    while (1) {
        scanf(" %c", &command);

        switch (command) {
        case '+':
            scanf(" %d %d %d", &u, &v, &w);
            // there's a -1 since we use 0-indexing in the arrays
            insertEdge(G, u - 1, v - 1, w);
            printf("Successfully inserted edge %d %d\n", u, v);
            break;
        case '#':
            scanf(" %d %d", &source, &target);
            dijkstra(G, source - 1, target - 1);
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
