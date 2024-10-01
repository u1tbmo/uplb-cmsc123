/**
 * @file tabamoejs_u1l_inlab_exer3.c
 * @author Euan Jed Tabamo
 * @brief This program implements a Binary Search Tree (BST) with basic insert
 * and search functions.
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
void updateHeight(BST_NODE *node);
int calculateTreeSize(BST_NODE *node);
void freeTree(BST_NODE *node);
void viewTreeStatus(BST *B);
BST_NODE *deleteNode(BST_NODE *node, int key);
BST_NODE *predecessor(BST_NODE *node);
BST_NODE *successor(BST_NODE *node);

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
 * @brief Creates a new BST node with the given key, left, right, and parent
 * pointers
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
        // Update the height of the node
        updateHeight(new);
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
 * @details The node is inserted into the BST in the correct position based on
 * the key, it also updates the height of the nodes in the path from the
 * inserted node's parent to the root
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
        B->root = node; // If the parent is NULL, then the new node is the root
                        // of the BST
    } else if (node->key < parent->key) {
        parent->left = node;
    } else {
        parent->right = node;
    }

    // Update the size of the BST
    B->size += calculateTreeSize(node);

    // Traverse upward from the inserted node's parent and update the height of
    // the nodes We do not need to update height of the node itself since it is
    // already updated in createBSTNode
    while (parent != NULL) {
        // Update the height of the parent
        updateHeight(parent);

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

    // Traverse the tree until the key is found or the end of the tree is
    // reached
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

/**
 * @brief Obtains the node which has the maximum key given a tree's root node.
 *
 * @param node the root node of the tree to find the maximum of
 * @return the node with the maximum key
 */
BST_NODE *maximum(BST_NODE *node) {
    // If the node is NULL, then the maximum is NULL.
    if (node == NULL) {
        return NULL;
    }
    // The maximum of a BST is always the right-most node.
    if (node->right != NULL) {
        return maximum(node->right);
    }
    return node;
}

/**
 * @brief Obtains the node which has the minimum key given a tree's root node.
 *
 * @param node the root node of the tree to find the minimum of
 * @return the node with the minimum key
 */
BST_NODE *minimum(BST_NODE *node) {
    // If the node is NULL, then the minimum is NULL.
    if (node == NULL) {
        return NULL;
    }
    // The minimum of a BST is always the left-most node.
    if (node->left != NULL) {
        return minimum(node->left);
    }
    return node;
}

int delete(BST *B, int key) {
    if (isEmpty(B)) {
        printf("Tree is empty.\n");
        return key;
    }

    // Pre-check whether the key exists
    int exists = (search(B, key) != NULL);

    // Decrement the tree size if the node with the key exists
    if (exists) {
        B->size--;
    }

    // Obtain the updated root node after deleting the desired node
    B->root = deleteNode(B->root, key);
    

    // Currently unused
    return key;
}

void clear(BST *B) {
    // Clear the tree nodes then clear the BST itself
    freeTree(B->root);
    free(B);
}

// Traversal Functions

/**
 * @brief Prints the keys of the BST in pre-order traversal given the root node
 *
 * @param node The root node of the tree to traverse
 */
void preorderWalkHelper(BST_NODE *node) {
    if (node == NULL) {
        return;
    }

    printf("%d ", node->key);
    preorderWalkHelper(node->left);
    preorderWalkHelper(node->right);
}

/**
 * @brief Prints the keys of the BST in pre-order traversal given the BST
 *
 * @param B the BST to traverse
 */
void preorderWalk(BST *B) {
    if (isEmpty(B)) {
        printf("The tree is empty.\n");
        return;
    }
    preorderWalkHelper(B->root);
}

/**
 * @brief Prints the keys of the BST in in-order traversal given the root node
 *
 * @param node the root node of the tree to traverse
 */
void inorderWalkHelper(BST_NODE *node) {
    if (node == NULL) {
        return;
    }

    inorderWalkHelper(node->left);
    printf("%d ", node->key);
    inorderWalkHelper(node->right);
}

/**
 * @brief Prints the keys of the BST in in-order traversal given the BST
 *
 * @param B the BST to traverse
 */
void inorderWalk(BST *B) {
    if (isEmpty(B)) {
        printf("The tree is empty.\n");
        return;
    }

    inorderWalkHelper(B->root);
}

/**
 * @brief Prints the keys of the BST in post-order traversal given the root node
 *
 * @param node the root node of the tree to traverse
 */
void postorderWalkHelper(BST_NODE *node) {
    if (node == NULL) {
        return;
    }

    postorderWalkHelper(node->left);
    postorderWalkHelper(node->right);
    printf("%d ", node->key);
}

/**
 * @brief Prints the keys of the BST in post-order traversal given the BST
 *
 * @param B the BST to traverse
 */
void postorderWalk(BST *B) {
    if (isEmpty(B)) {
        printf("The tree is empty.\n");
        return;
    }

    postorderWalkHelper(B->root);
}

// Predecessor and Successor Functions

BST_NODE *predecessor(BST_NODE *node) {
    // The predecessor is defined as the maximum of the left subtree of the
    // node.
    if (node != NULL) {
        return maximum(node->left);
    } else {
        printf("Node is null!\n");
    }
}

BST_NODE *successor(BST_NODE *node) {
    // The successor is defined as the minimum of the right subtree of the node.
    if (node != NULL) {
        return minimum(node->right);
    } else {
        printf("Node is null!\n");
    }
}

// Helper Functions

/**
 * @brief Updates the height of a node
 *
 * @param node the node to update
 */
void updateHeight(BST_NODE *node) {
    int lHeight = (node->left != NULL) ? node->left->height : -1;
    int rHeight = (node->right != NULL) ? node->right->height : -1;
    node->height = 1 + ((lHeight > rHeight) ? lHeight : rHeight);
}

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
 * @brief View the status of the BST, including the size, max size, root, and
 * height
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
 * @brief Deletes the node with the given key from a tree given its root node
 *
 * @param node the root node of the tree to delete from
 * @param key the key of the node to delete
 * @return the root node of the updated tree after node deletion
 */
BST_NODE *deleteNode(BST_NODE *node, int key) {
    // +20+30+10+9+5+25+8p-25p-9p-20p
    BST_NODE *temp;
    // If the node is null, then there is nothing to remove
    if (node == NULL) {
        return NULL;
    }

    // Traverse recursively to the parent of the node to delete
    // We set each node's child (on the path to the node to delete)
    // to the recursive call's resulting subtree
    if (key < node->key) {
        node->left = deleteNode(node->left, key);
    } else if (key > node->key) {
        node->right = deleteNode(node->right, key);
    } else {
        // Case 1a: Leaf node or one child right
        // If node->left is NULL, we simply make node->right
        // the root of the subtree.
        // This works even if node->right is NULL, handling leaf nodes.
        if (node->left == NULL) {
            temp = node->right;
            if (temp != NULL) {
                temp->parent = node->parent;
            }
            free(node);
            return temp;
        }
        // Case 1b: One child left
        // If node->right == NULL, we simply make node->left the root of the
        // subtree. Since to node->left is not null, node->left always exists
        else if (node->right == NULL) {
            temp = node->left;
            temp->parent = node->parent;
            free(node);
            return temp;
        }
        // Case 2: Two Children
        // PREDECESSOR DELETION
        node->key = predecessor(node)->key;
        node->left = deleteNode(node->left, node->key);
        // SUCCESSOR DELETION
        /* node->key = successor(node)->key;
        node->right = deleteNode(node->right, node->key); */
    }

    // Update the height of every node traversed backwards
    updateHeight(node);
    // Return the root node to the caller.
    return node;
}

/**
 * @brief Frees the BST node and its children recursively given the root node of
 * the tree to free.
 *
 * @param node the root node to free along with its children
 */
void freeTree(BST_NODE *node) {
    // Base case: if the node is NULL, then do nothing
    if (node == NULL) {
        return;
    }

    // Free the left and right children recursively
    freeTree(node->left);
    freeTree(node->right);

    // Free the node itself
    free(node);
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
        case '-':
            scanf("%d", &key);
            printf("Removing node with key: %d\n", key);
            result = delete (B, key); // result is unused. print if u want
            break;
        case '?':
            scanf(" %d", &key);
            printf("Searching node with key: %d. Location: %p\n", key,
                   search(B, key));
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

        case 'Q':
            clear(B);
            return 0;
        default:
            printf("Unknown command: %c\n", command);
        }
    }

    return 0;
}
