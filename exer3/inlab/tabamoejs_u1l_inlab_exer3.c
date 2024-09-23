/**
 * @file tabamoejs_u1l_inlab_exer3.c
 * @author Euan Jed Tabamo
 * @brief This program implements a Binary Search Tree (BST) with basic insert and search functions.
 * @version 0.1
 * @date 2024-09-21
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "BST.h"
#include <stdio.h>
#include <stdlib.h>

// Prototypes
int calculateTreeSize(BST_NODE *node);
void freeBSTNode(BST_NODE *node);
void freeBST(BST *B);
void viewTreeStatus(BST *B);

/**
 * @brief A recursive helper function to show the BST in tree mode.
 *
 * @param node The root node of any tree to show.
 * @param tabs The number of tabs to show before the node for spacing.
 */
void showTreeHelper(BST_NODE *node, int tabs) {

    if (!node)
        return; // node is null, do nothing
    showTreeHelper(node->right, tabs + 1);
    for (int i = 0; i < tabs; i++)
        printf("\t");
    printf("%d(%d)\n", node->key, node->height);
    showTreeHelper(node->left, tabs + 1);
}

/**
 * @brief Shows the BST in tree mode
 *
 * @param B the non-null BST to show
 */
void showTree(BST *B) { showTreeHelper(B->root, 0); }

/**
 * @brief Creates a new BST node with the given key, left, right, and parent pointers
 *
 * @param key the integer key of the new node
 * @param L the left child of the new node
 * @param R the right child of the new node
 * @param P the parent of the new node
 * @return the newly created BST node pointer
 */
BST_NODE *createBSTNode(int key, BST_NODE *L, BST_NODE *R, BST_NODE *P) {
    // Allocate memory for the new node
    BST_NODE *new = (BST_NODE *)malloc(sizeof(BST_NODE));

    // Check if memory allocation failed
    if (new == NULL) {
        return NULL;
    }

    // Initialize the new node
    *new = (BST_NODE){
        .left = L,
        .right = R,
        .parent = P,
        .height = 0,
        .key = key,
    };

    // Set the parents of the left and right child nodes to the new node
    // This is necessary to maintain the parent-child relationship, otherwise,
    // It is impossible to traverse upward from the children to the parent
    if (L != NULL) {
        L->parent = new;
    }
    if (R != NULL) {
        R->parent = new;
    }

    // Check if the node has children, and if so, set the height correctly
    if (L != NULL || R != NULL) {
        // Measure the height of the left and right children, -1 if the child is NULL
        int leftHeight = L == NULL ? -1 : L->height;
        int rightHeight = R == NULL ? -1 : R->height;
        // Set the height of the new node to 1 plus the maximum height of the left and right children
        new->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
    }

    // Return the new node
    return new;
}

/**
 * @brief Creates an empty new BST with the given maximum size
 *
 * @param max the integer maximum size of the BST
 * @return the newly created BST's pointer
 */
BST *createBST(int max) {
    // Allocate memory for the new BST
    BST *new = (BST *)malloc(sizeof(BST));

    // Check if memory allocation failed
    if (new == NULL) {
        return NULL;
    }

    // Initialize the new BST
    *new = (BST){
        .root = NULL,
        .maxSize = max,
        .size = 0,
    };

    // Return the new BST
    return new;
}

/**
 * @brief Checks if the BST is empty
 *
 * @param B the non-null BST to check
 * @return 1 if the BST is empty, 0 if not empty
 */
int isEmpty(BST *B) { return B->root == NULL; }

/**
 * @brief Checks if the BST is full
 *
 * @param B the non-null BST to check
 * @return 1 if the BST is full, 0 if not full
 */
int isFull(BST *B) { return B->size == B->maxSize; }

/**
 * @brief Inserts a node into the BST
 * @details The node is inserted into the BST in the correct position based on the key, it also updates the height of
 *          the nodes in the path from the inserted node's parent to the root
 * @param B the non-null BST to insert into
 * @param node the node to insert into the BST
 */
void insert(BST *B, BST_NODE *node) {
    // If the node is NULL, then insertion is impossible
    if (node == NULL) {
        return;
    }

    // If the BST is full, then insertion is impossible
    if (isFull(B)) {
        printf("BST is Full!\n");
        return;
    }

    // Initialize necessary pointers
    BST_NODE *parent = NULL;
    BST_NODE *current = B->root;

    // Traverse to the correct leaf node to insert the new node
    while (current != NULL) {
        // Let the parent node traverse behind current node
        parent = current;

        // Handle duplicate keys by ignoring the insertion
        if (node->key == current->key) {
            printf("Key %d already exists in the BST!\n", node->key);
            // Free the node subtree to prevent memory leaks
            freeBSTNode(node);
            return;
        }
        // Traverse left or right depending on the key
        if (node->key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // Set the new node's parent to the parent node
    node->parent = parent;

    // Insert the node to the position of the current node
    if (node->parent == NULL) {
        B->root = node; // If the parent is NULL, then the new node is the root of the BST
    } else if (node->key < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    // Update the size of the BST
    B->size += calculateTreeSize(node);

    // Traverse upward from the inserted node's parent and update the height of the nodes
    // We do not need to update height of the node itself since it is already updated in createBSTNode
    while (parent != NULL) {
        // Measure the height of the left and right children, -1 if the child is NULL
        int leftHeight = parent->left == NULL ? -1 : parent->left->height;
        int rightHeight = parent->right == NULL ? -1 : parent->right->height;

        // Update the height of the parent node, which is 1 plus the maximum height of the left and right children
        parent->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

        // Traverse upward the tree
        parent = parent->parent;
    }
}

/**
 * @brief Searches for a node with the given key in the BST
 *
 * @param B the non-null BST to search in
 * @param key the integer key of the node to search for
 * @return the node pointer with the given key if found, otherwise NULL
 */
BST_NODE *search(BST *B, int key) {
    // Start searching from the root node
    BST_NODE *current = B->root;

    // Traverse the tree until the key is found or the end of the tree is reached
    while (current != NULL) {
        if (key == current->key) {
            return current;
        } else if (key < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }

    // If the key is not found, return NULL
    return NULL;
}

// New Helper Functions

/**
 * @brief Calculates the size of the tree recursively
 *
 * @param node the root node of the tree to calculate the size of
 * @return the size of the tree
 */
int calculateTreeSize(BST_NODE *node) {
    // Base Case: if the node is NULL, then the size is 0
    if (node == NULL) {
        return 0;
    }
    // Recursive Case: return 1 plus the size of the left and right children
    return 1 + calculateTreeSize(node->left) + calculateTreeSize(node->right);
}

/**
 * @brief View the status of the BST, including the size, max size, root, and height
 *
 * @param B the BST to view the status of
 */
void viewTreeStatus(BST *B) {
    printf("Size: %d\n", B->size);
    printf("Max Size: %d\n", B->maxSize);
    if (B->root != NULL) {
        printf("Root: %d\n", B->root->key);
        printf("Height: %d\n", B->root->height);
    } else {
        printf("Root: NULL\n");
        printf("Height: -1\n");
    }
}

/**
 * @brief Frees the BST node and its children recursively
 *
 * @param node the root node to free along with its children
 */
void freeBSTNode(BST_NODE *node) {
    // Base case: if the node is NULL, then do nothing
    if (node == NULL) {
        return;
    }

    // Recursive case: free the left and right children
    freeBSTNode(node->left);
    freeBSTNode(node->right);

    // Free the node itself
    free(node);
}

/**
 * @brief Free the BST and its nodes
 *
 * @param B the BST to free
 */
void freeBST(BST *B) {
    freeBSTNode(B->root);
    free(B);
}

/**
 * @brief Main function to run the program
 *
 * @return the exit status of the program
 */
int main() {
    char command;
    int key, result;

    BST *B = createBST(100);
    BST_NODE *node;
    while (1) {
        scanf(" %c", &command);

        switch (command) {
        case '+':
            scanf("%d", &key);
            printf("Inserting key: %d\n", key);
            insert(B, createBSTNode(key, NULL, NULL, NULL));
            break;
        case '*': // a special case for inserting a node that has left and right children
            // requires the user three keys IN ASCENDING ORDER or else
            // the binary search tree property will be violated
            int leftKey, rightKey;
            scanf("%d %d %d", &leftKey, &key, &rightKey);
            printf("Inserting keys: %d %d %d\n", leftKey, key, rightKey);
            // Let the left and right children be created first
            BST_NODE *left = createBSTNode(leftKey, NULL, NULL, NULL);
            BST_NODE *right = createBSTNode(rightKey, NULL, NULL, NULL);
            // Create the node with the left and right children and insert to the BST
            node = createBSTNode(key, left, right, NULL);
            insert(B, node);
            break;
        case '?':
            scanf(" %d", &key);
            printf("Searching node with key: %d. Location: %p\n", key, search(B, key));
            // (nil) means NULL pointer
            break;
        case 'p':
            printf("Tree (rotated +90 degrees): \n");
            showTree(B);
            printf("\n");
            break;
        case 'S': // a case to view the status of the BST
            viewTreeStatus(B);
            break;
        case 'E':
            printf("BST %s empty.\n", isEmpty(B) ? "is" : "is not");
            break;
        case 'F':
            printf("BST %s full.\n", isFull(B) ? "is" : "is not");
            break;
        /* for the postlab, uncomment this
        case '<':
                printf("Pre-order Traversal: ");
                preorderWalk(B);
                printf("\n");
                break;
        case '>':
                printf("Post-order Traversal: ");
                postorderWalk(B);
                printf("\n");
                break;
        case '/':
                printf("In-order Traversal: ");
                inorderWalk(B);
                printf("\n");
                break;
        */
        case 'Q':
            freeBST(B);
            return 0;
        default:
            printf("Unknown command: %c\n", command);
        }
    }

    return 0;
}