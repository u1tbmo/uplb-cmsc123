/**
 * @author Euan Jed Tabamo
 * @brief This program implements an AVL Tree with basic insert functionality.
 * @version 1.0
 * @date 2024-10-08
 *
 * @copyright Copyright (c) 2024
 *
 */

/* ********************************************************* *
 * template.c                                                *
 *                                                           *
 * Template C Source File for AVL Insert.                    *
 *                                                           *
 * !!! DO NOT EDIT THE CONTENTS OF THE MAIN FUNCTION !!!     *
 *                                                           *
 * ********************************************************* */
#include "AVL.h"
#include <stdio.h>
#include <stdlib.h>

AVL_NODE *createAVLNode(int key) {
    return createBSTNode(key, NULL, NULL, NULL);
}

AVL *createAVL(int max) { return createBST(max); }

/**** PUT YOUR FUNCTIONS HERE ******************************/

/**
 * @brief Calculates the balance factor of a node.
 *
 * @param node the node to calculate the balance factor of
 */
int balanceFactorOf(AVL_NODE *node) {
    return ((node != NULL) ? heightOf(node->left) - heightOf(node->right) : 0);
}

/**
 * @brief Rotates the subtree induced by a node to the left
 *
 * @param node the root node of the subtree to rotate
 * @return the root node of the tree after rotation
 */
AVL_NODE *leftRotateHelper(AVL_NODE *node) {
    // pivot is rightChild
    // pivot->left is grandChild
    AVL_NODE *pivot = node->right;

    pivot->parent = node->parent;

    // Transfer the grandchild to the critical node if it exists
    node->right = pivot->left;
    if (pivot->left != NULL) {
        pivot->left->parent = node;
    }

    // Make the pivot the root of the tree
    pivot->left = node;
    node->parent = pivot;

    // Update heights
    updateHeight(node);
    updateHeight(pivot);

    // Return the pivot node as the root
    return pivot;
}

/**
 * @brief Rotates the subtree induced by a node to the right
 *
 * @param node the root node of the subtree to rotate
 * @return the root node of the tree after rotation
 */
AVL_NODE *rightRotateHelper(AVL_NODE *node) {
    // pivot is leftChild
    // pivot->right is grandChild
    AVL_NODE *pivot = node->left;

    pivot->parent = node->parent;

    // Transfer the grandchild to the critical node if it exists
    node->left = pivot->right;
    if (pivot->right != NULL) {
        pivot->right->parent = node;
    }

    // Make the pivot the root of the tree
    pivot->right = node;
    node->parent = pivot;

    // Update heights
    updateHeight(node);
    updateHeight(pivot);

    // Return the pivot node as the root
    return pivot;
}

/**
 * @brief Rotates the subtree induced by a node in a tree to the left
 *
 * @param A the tree in which the subtree is located
 * @param node the root node of the subtree to rotate
 */
void leftRotate(AVL *A, AVL_NODE *node) {
    if (node == NULL) {
        return;
    }

    // Get the subtree's new root by rotating
    // the subtree induced by node
    AVL_NODE *newRoot = leftRotateHelper(node);

    // If the new root's parent is NULL, then it must be the root of the tree
    if (newRoot->parent == NULL) {
        A->root = newRoot;
    }
    // Otherwise, replace the nodeToRotate with the newRoot
    else {
        if (newRoot->parent->left == node) {
            newRoot->parent->left = newRoot;
        } else {
            newRoot->parent->right = newRoot;
        }
    }
}

/**
 * @brief Rotates the subtree induced by a node in a tree to the right
 *
 * @param A the tree in which the subtree is located
 * @param node the root node of the subtree to rotate
 */
void rightRotate(AVL *A, AVL_NODE *node) {
    if (node == NULL) {
        return;
    }

    // Get the subtree's new root by rotating
    // the subtree induced by nodeToRotate
    AVL_NODE *newRoot = rightRotateHelper(node);

    // If the new root's parent is NULL, then it must be the root of the tree
    if (newRoot->parent == NULL) {
        A->root = newRoot;
    }
    // Otherwise, replace the nodeToRotate with the newRoot
    else {
        if (newRoot->parent->left == node) {
            newRoot->parent->left = newRoot;
        } else {
            newRoot->parent->right = newRoot;
        }
    }
}

/**
 * @brief Obtains the height of a node
 *
 * @param node the node to calculate the height of
 * @return the height of the given node
 */
int heightOf(AVL_NODE *node) {
    // The height is -1 if the node is null
    if (node == NULL) {
        return -1;
    }
    // Otherwise the height is 1 + max(node->left->height, node->right->height)
    int lHeight = (node->left != NULL) ? node->left->height : -1;
    int rHeight = (node->right != NULL) ? node->right->height : -1;
    return 1 + ((lHeight > rHeight) ? lHeight : rHeight);
}

/**
 * @brief Inserts a node into an AVL tree
 * 
 * @param A the AVL tree to insert the node into
 * @param node the node to insert into the AVL tree
 */
void AVLInsert(AVL *A, AVL_NODE *node) {
    // TEST: +39p+25p+32p+45p+49p+35p+30p+27p+31p+47p
    // Impossible Insertion Cases
    if (node == NULL) {
        return;
    }
    if (isFull(A)) {
        printf("BST is Full!\n");
        return;
    }

    // Initialize necessary pointers
    AVL_NODE *parent = NULL;
    AVL_NODE *current = A->root;

    // Traverse to the correct leaf node to insert the new node
    while (current != NULL) {
        // Let the parent node traverse behind current node
        parent = current;

        // Handle duplicate keys by ignoring the insertion
        if (node->key == current->key) {
            printf("Key %d already exists in the AVL Tree!\n", node->key);
            // Free the node subtree to prevent memory leaks
            freeTree(node);
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
        A->root = node; // If the parent is NULL, then the new node is the root
                        // of the AVL tree
    } else if (node->key < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    // Update the size of the AVL tree
    A->size += calculateTreeSize(node);

    // Traverse upward from the inserted node's parent
    // and update the height of the nodes and rebalance each unbalanced node.
    while (parent != NULL) {
        // Update the height of the node
        updateHeight(parent);
        
        // Calculate the balance of the node and check if it is a critical node
        // The sign of the balance factor indicates the imbalance direction
        // If balance is negative, then the right subtree is larger
        // If balance is positive, then the left subtree is larger
        int balance = balanceFactorOf(parent);

        // Case 1: Left-left and left-right leaning
        if (balance > 1) {
            // Case 1b: If left-right leaning, then leftRotate the pivot first
            if (balanceFactorOf(parent->left) < 0) {
                leftRotate(A, parent->left);
            }
            // Right rotate the critical node
            rightRotate(A, parent);
        }
        // Case 2: Right-right and right-left leaning
        else if (balance < -1) {
            // Case 2b: If right-left leaning, then rightRotate the pivot first
            if (balanceFactorOf(parent->right) > 0) {
                rightRotate(A, parent->right);
            }
            // Left rotate the critical node
            leftRotate(A, parent);
        }

        // Traverse upward the tree
        parent = parent->parent;
    }
}

/**** FUNCTIONS ABOVE **************************************/

int main() {

    char command;
    int key, result;

    AVL *A = createAVL(100);
    AVL_NODE *node;
    while (1) {
        scanf(" %c", &command);

        switch (command) {
        case '+':
            scanf("%d", &key);
            printf("Inserting key: %d\n", key);
            AVLInsert(A, createAVLNode(key));
            break;
        case 'p':
            printf("Tree (rotated +90 degrees): \n");
            showTree(A);
            printf("\n");
            break;
        case 'C':
            printf("Removing all contents.\n");
            clear(A);
            break;
        case 'Q':
            clear(A);
            free(A);
            return 0;
        default:
            printf("Unknown command %c\n", command);
        }
    }
}