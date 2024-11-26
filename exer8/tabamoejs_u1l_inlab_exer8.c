#include "hashtable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

STRING copyString(STRING str);

// Function that prints the data of the hash table
void printTable(HASH_TABLE_PTR H) {
    if (isEmpty(H)) {
        printf("*empty\n");
        return;
    }

    for (int i = 0; i < H->tableSize; i++) {
        printf("Cell#%d: ", i);
        if (H->list[i] != NULL) {
            if (H->list[i][0] != '\0') {
                char *copy = copyString(H->list[i]); // Copy the string before using strtok
                char *key = strtok(copy, ":");       // Get the key
                char *data = strtok(NULL, ":");      // Get the data
                printf("%s\n", data);                // Print the data
                free(copy);                          // Free the copied string
            } else {
                printf("*deleted*\n"); // If the cell is marked as deleted, print *deleted*
            }
        } else {
            printf("*empty*\n"); // If the cell is empty, print *empty*
        }
    }
}

// Utility function

/**
 * Allocate memory for a copy of a string
 *
 * @param str the string to be copied
 * @return the copied string
 */
STRING copyString(STRING str) {
    STRING copy = (STRING)malloc(sizeof(char) * strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

// Create your own hash functions

/**
 * Compute the key of a string based on the sum of the ASCII values of the even characters
 *
 * @param key the string key to be hashed
 * @return the computed key
 */
int computeKey(STRING key) {
    int result = 0;

    // Iterate over the key
    for (int i = 0; key[i] != '\0'; i++) {
        // Add the ASCII value of the character if it is even
        result += (key[i] % 2 == 0) ? key[i] : 0;
    }

    return result;
}

/**
 * The primary hash function
 *
 * @param key the string key to be hashed
 * @param size the size of the hash table
 * @return the hashed value
 */
int hash1(STRING key, int size) {
    // Modulo type hash function
    return computeKey(key) % size;
}

/**
 * The secondary hash function for handling collisions
 *
 * @param key the string key to be hashed
 * @param collisions the number of collisions that have occurred
 * @return the hashed value
 */
int hash2(STRING key) {
    // Linear probing type hash function
    return 17 + computeKey(key) % strlen(key);
}

/**
 * Computes the hash value of a string key with double hashing
 *
 * @param key the string key to be hashed
 * @param size the size of the hash table
 * @param collisions the number of collisions that have occurred
 * @return the hashed value
 */
int hash(STRING key, int size, int collisions) { return (hash1(key, size) + collisions * hash2(key)) % size; }

// Insert the implementations of the functions found in the header file

HASH_TABLE_PTR createHashTable(int tableSize) {
    // Allocate memory for the hash table
    HASH_TABLE_PTR H = (HASH_TABLE_PTR)malloc(sizeof(HASH_TABLE));
    if (H == NULL) {
        return NULL;
    }

    // Initialize the hash table
    H->size = 0;
    H->tableSize = tableSize;

    // Allocate memory for the list of strings
    H->list = (STRING_ARRAY_PTR)malloc(sizeof(STRING) * H->tableSize);

    // Initialize the list
    for (int i = 0; i < H->tableSize; i++) {
        H->list[i] = NULL;
    }

    return H;
}

int isEmpty(HASH_TABLE_PTR H) { return H->size == 0; }

int isFull(HASH_TABLE_PTR H) { return H->size == H->tableSize; }

void put(HASH_TABLE_PTR H, STRING key, STRING data) {
    // You cannot insert into a full table
    if (isFull(H)) {
        return;
    }

    // Keep track of the number of probes/collisions and the index
    int c = 0;
    int index;

    // Probe until an empty cell is found
    while (1) {
        index = hash(key, H->tableSize, c++);
        if (H->list[index] == NULL) {
            // Insert the pair at the empty cell
            H->list[index] = (STRING)malloc(sizeof(char) * (strlen(key) + strlen(data) + 3));
            sprintf(H->list[index], "%s:%s", key, data);
            break;
        }

        else if (H->list[index][0] == '\0') {
            // Insert the pair at the deleted cell
            free(H->list[index]);
            H->list[index] = (STRING)malloc(sizeof(char) * (strlen(key) + strlen(data) + 3));
            sprintf(H->list[index], "%s:%s", key, data);
            break;
        }

        // Don't insert if the key is already in the table
        if (strcmp(H->list[index], key) == 0 && H->list[index][strlen(key)] == ':') {
            return;
        }
    }

    // Increment the size
    H->size++;
}

STRING find(HASH_TABLE_PTR H, STRING key, STRING data) {
    // NOTE: The data parameter is not used in this function
    // This is because the data is not needed to find the key and using data to search is not standard behavior.
    // It also kind of defeats the purpose of a hash table since you are not using the key to find the data.

    // You cannot find from an empty table
    if (isEmpty(H)) {
        return NULL;
    }

    // Keep track of the number of probes/collisions and the index
    int c = 0;
    int index;
    while (c < H->tableSize) {
        index = hash(key, H->tableSize, c++); // Get the index

        if (H->list[index] == NULL) { // If the cell is empty, the data is not in the table
            return NULL;
        }

        if (H->list[index][0] == '\0') { // If the cell is marked as deleted, skip the entry
            continue;
        }

        char *copy = copyString(H->list[index]); // Copy the string before using strtok
        char *k = strtok(copy, ":");             // Get the key
        char *d = strtok(NULL, ":");             // Get the data

        // If the key is found,
        if (strcmp(key, k) == 0) {
            free(copy);
            return H->list[index];
        }
        free(copy);
    }

    return NULL;
}

STRING erase(HASH_TABLE_PTR H, STRING key, STRING data) {
    // NOTE: The data parameter is not used in this function.
    // This is because the data is not needed to erase the key and using data to erase is not standard behavior.
    // In main, since only the key is asked from the user for the erase case, even if data is considered for erasing,
    //    it would be possible for this function to not delete the entry even if the key is valid and exists in the table.

    // You cannot erase from an empty table
    if (isEmpty(H)) {
        return NULL;
    }

    // Keep track of the number of probes/collisions and the index
    int c = 0;
    int index;
    char *result;
    while (c < H->tableSize) {
        index = hash(key, H->tableSize, c++);

        if (H->list[index] == NULL) { // If the cell is empty, the data is not in the table
            return NULL;
        }

        if (H->list[index][0] == '\0') { // If the cell is marked as deleted, skip the entry
            continue;
        }

        char *copy = copyString(H->list[index]); // Copy the string before using strtok
        char *k = strtok(copy, ":");             // Get the key
        char *d = strtok(NULL, ":");             // Get the data

        // If the key is found,
        if (strcmp(key, k) == 0) {
            result = copyString(H->list[index]);
            free(copy);

            // Free the memory of the data and mark as deleted
            free(H->list[index]);
            H->list[index] = (STRING)malloc(sizeof(char));
            H->list[index][0] = '\0';

            // Decrement the size
            H->size--;

            return result;
        }
        free(copy);
    }

    return NULL;
}

void destroy(HASH_TABLE_PTR H) {
    if (H != NULL) {
        // Clear all memory allocated for the nodes
        for (int i = 0; i < H->tableSize; i++) {
            if (H->list[i] != NULL) {
                free(H->list[i]);
                H->list[i] = NULL;
            }
        }
        // Reset the size
        H->size = 0;
    }
}

int main() {

    char command;
    STRING key;
    STRING data;
    STRING result;

    unsigned int tsize;
    HASH_TABLE_PTR H;

    // get table size then maxsize
    scanf("%d\n", &tsize);
    H = createHashTable(tsize);

    // assume max key size is 20
    key = (STRING)malloc(sizeof(char) * 20);
    key[0] = '\0';
    // assume max data size is 100
    data = (STRING)malloc(sizeof(char) * 100);
    data[0] = '\0';

    while (1) {
        scanf(" %c", &command);

        switch (command) {
        case '+':
            scanf(" k:%s d:%s", key, data);
            printf("Inserting data %s with key %s\n", data, key);
            put(H, key, data);
            break;
        case '-':
            scanf(" %s", key);
            printf("Deleting data with key %s\n", key);
            result = erase(H, key, data);
            if (result != NULL) {
                free(result); // free the memory of the data
            }
            break;
        case '?':
            scanf(" k:%s d:%s", key, data);
            printf("Searching data with key: %s. Location: %p\n", key, find(H, key, data));
            // (nil) means NULL pointer
            break;
        case 'p':
            printf("Hash Table: \n");
            printTable(H);
            printf("\n");
            break;
        case 'E':
            printf("Hash table %s empty.\n", isEmpty(H) ? "is" : "is not");
            break;
        case 'F':
            printf("Hash table %s full.\n", isFull(H) ? "is" : "is not");
            break;
        case 'C':
            printf("Deleting all contents.\n");
            destroy(H);
            break;
        case 'Q':
            free(key);
            free(data);
            destroy(H); // ensure deletion
            free(H->list);
            free(H);
            return 0;
        default:
            printf("Unknown command: %c\n", command);
        }
    }

    return 0;
}
