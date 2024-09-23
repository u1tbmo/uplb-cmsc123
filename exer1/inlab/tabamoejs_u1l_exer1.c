/* Tabamo, Euan Jed S. - CMSC 123 U-1L
** Exercise 1 - Stacks and Queues
** Description: Checks if a program has a pair of parentheses inside a pair of brackets.
** Date Created: August 27, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include "hasParenthesis.h"

/*
** peek()
** requirements: a list
** results:
	returns the value of the 'head' node
    does not change the list
*/
int peek(LIST *L);

/*
** get_string()
** requirements: a prompt string and FILE stream to read from
** results:
	returns the string read from the FILE stream
*/
char *get_string(char *prompt, FILE *stream);

int main()
{
    // Sample Cases
    printf("Test 1 ---------------\n");
    int test1 = hasParenthesis("[x+y]");
    printf("Result: %s", (test1 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 2 ---------------\n");
    int test2 = hasParenthesis("[(2+x)-(2+10)]");
    printf("Result: %s", (test2 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 3 ---------------\n");
    int test3 = hasParenthesis("[9-2]+(10/3)");
    printf("Result: %s", (test3 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 4 ---------------\n");
    int test4 = hasParenthesis("[x+2]+[(6^2)/3]");
    printf("Result: %s", (test4 == 1) ? "Found\n\n" : "Not Found\n\n");

    // Additional Valid Cases
    // All parenthesis and brackets properly matched, no nested brackets
    printf("Test 5 ---------------\n");
    int test5 = hasParenthesis("[x]+(y)+[z]");
    printf("Result: %s", (test5 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 6 ---------------\n");
    int test6 = hasParenthesis("[x+(y+z)]");
    printf("Result: %s", (test6 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 7 ---------------\n");
    int test7 = hasParenthesis("[1]+[2]+([3])");
    printf("Result: %s", (test7 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 8 ---------------\n");
    int test8 = hasParenthesis("([x+y]+z)");
    printf("Result: %s", (test8 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 9 ---------------\n");
    int test9 = hasParenthesis("[(x)]+[(y)]+([])");
    printf("Result: %s", (test9 == 1) ? "Found\n\n" : "Not Found\n\n");

    // Invalid Cases
    // Umatched parentheses or brackets or improper closing
    printf("Test 10 --------------\n");
    int test10 = hasParenthesis("[(]");
    printf("Result: %s", (test10 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 11 --------------\n");
    int test11 = hasParenthesis("[)]");
    printf("Result: %s", (test11 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 12 --------------\n");
    int test12 = hasParenthesis("[)()]");
    printf("Result: %s", (test12 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 13 --------------\n");
    int test13 = hasParenthesis("(][)");
    printf("Result: %s", (test13 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 14 --------------\n");
    int test14 = hasParenthesis("(][()]");
    printf("Result: %s", (test14 == 1) ? "Found\n\n" : "Not Found\n\n");
    printf("Test 15 --------------\n");
    int test15 = hasParenthesis("[()][)");
    printf("Result: %s", (test15 == 1) ? "Found\n\n" : "Not Found\n\n");

    // Menu (if needed)
    int choice = -1;
    char *expression = NULL;
    while (choice != 0)
    {
        printf("========================\n");
        printf("[1] Test Expression\n");
        printf("[0] Exit\n");
        printf("========================\n");
        printf("Choice: ");
        scanf("%d", &choice);

        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ; // clear the input buffer

        switch (choice)
        {
        case 1:
            expression = get_string("Enter your expression: ", stdin);
            printf("Result: %s", (hasParenthesis(expression) == 1) ? "Found\n\n" : "Not Found\n\n");
            free(expression);
            break;
        case 0:
            printf("Goodbye!\n");
            break;
        default:
            printf("Error: Invalid choice\n");
            break;
        }
    }
}

// This implementation is from my CMSC 21 project with minor changes for exit().
char *get_string(char *prompt, FILE *stream)
{
    // Variables
    int buffer_length = 16, c, i = 0;
    char *temp;

    // Print prompt if there is a prompt
    if (prompt != NULL)
    {
        printf("%s", prompt);
    }

    // Allocate memory for a string
    char *buffer = (char *)malloc(sizeof(char) * buffer_length);
    if (buffer == NULL) // If malloc failed
    {
        printf("Oops! Memory allocation failed.\n\n");
        exit(EXIT_FAILURE);
    }

    while ((c = fgetc(stream)) != '\n' && c != EOF) // Read characters from the file stream one by one
    {
        // Fill the buffer with the characters from the file stream
        buffer[i++] = c;

        // If we've reached the end of the buffer, reallocate more memory
        if (i == buffer_length)
        {
            buffer_length *= 2;                                           // Increase size of buffer
            temp = (char *)realloc(buffer, sizeof(char) * buffer_length); // Reallocate memory
            if (temp == NULL)                                             // If realloc failed.
            {
                printf("Oops! Memory allocation failed.\n\n");
                free(buffer);
                exit(EXIT_FAILURE);
            }
            else
            {
                buffer = temp; // Point the buffer to the new memory (if it ever moved)
            }
        }
    }

    // Null-terminate the string
    buffer[i] = '\0';

    // Shrink the buffer to the actual size
    temp = (char *)realloc(buffer, sizeof(char) * (i + 1));
    if (temp == NULL) // If realloc fails
    {
        printf("Oops! Memory allocation failed.\n\n");
        free(buffer);
        exit(EXIT_FAILURE);
    }
    buffer = temp; // The buffer is resized to the optimal size

    return buffer; // Return the string
}

void printStack(LIST *L)
{
    // Case 1: LIST is empty
    if (L->head == NULL)
    {
        printf("*empty*\n");
        return;
    }

    // Case 2: LIST is not empty
    // Prints in format <node1><node2>...
    NODE *curr = L->head;
    while (curr != NULL)
    {
        printf("%c", curr->value);
        curr = curr->next;
    }
    printf("\n");
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

LIST *createStack()
{
    // Allocate memory for a LIST
    LIST *new = (LIST *)malloc(sizeof(LIST));

    if (new == NULL)
    {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    // Initialize new LIST
    new->head = NULL;

    return new;
}

int isEmpty(LIST *L)
{
    // If the head is NULL, then the LIST is empty
    return L->head == NULL;
}

void push(LIST *L, NODE *node)
{
    // Case 1: LIST is empty
    if (L->head == NULL)
    {
        L->head = node;
    }
    // Case 2: Prepends node to LIST
    else
    {
        node->next = L->head;
        L->head = node;
    }
}

int pop(LIST *L)
{
    // Check if the LIST is empty
    if (isEmpty(L))
    {
        printf("Error: Stack underflow\n");
        return -1;
    }
    // Return the value of the popped node
    else
    {
        NODE *temp = L->head;
        int value = temp->value;
        L->head = L->head->next;
        free(temp);
        return value;
    }
}

int peek(LIST *L)
{
    if (L->head == NULL)
    {
        return -1;
    }
    return L->head->value;
}

int hasParenthesis(char *expr)
{
    LIST *stack;
    NODE *new;
    int parenthesesPairFound = 0;              // Boolean to check if we found a parentheses pair
    int parenthesesPairFoundInBracketPair = 0; // Boolean to check if we found a parentheses pair inside a bracket pair

    // Create a Stack of Parentheses and Brackets
    stack = createStack();

    // Iterate over the string expr
    for (int i = 0; expr[i] != '\0'; i++)
    {
        // If we find [, push to the stack
        if (expr[i] == '[')
        {
            // Create a new node that will be pushed to the stack
            new = createNode(expr[i]);
            push(stack, new);

            parenthesesPairFound = 0;
        }

        // If we find (, push to the stack
        else if (expr[i] == '(')
        {
            // Create a new node that will be pushed to the stack
            new = createNode(expr[i]);
            push(stack, new);
        }

        // If we find ), pop ( from the stack
        else if (expr[i] == ')')
        {
            // Check for matching parenthesis
            if (peek(stack) == '(')
            {
                pop(stack);
            }
            else
            {
                printf("Error: Unmatched parenthesis\n");
                break; // Since the expression is invalid already
            }
            parenthesesPairFound = 1; // We now know that we have found a pair of parentheses.
        }

        // If we find ], pop [ from the stack
        else if (expr[i] == ']')
        {
            // Check for matching bracket
            if (peek(stack) == '[')
            {
                pop(stack);
            }
            else
            {
                printf("Error: Unmatched bracket\n");
                break; // Since the expression is invalid already
            }

            // If we found a parentheses pair before encountering ], the pair MUST be inside the bracket pair.
            if (parenthesesPairFound)
            {
                parenthesesPairFoundInBracketPair = 1;
            }
        }

        printStack(stack);
    }

    // Check if the expression is valid
    int expressionIsValid = isEmpty(stack);

    // Free memory from the stack
    while (stack->head != NULL)
    {
        pop(stack);
    }
    free(stack);

    // Return the result
    return expressionIsValid && parenthesesPairFoundInBracketPair;
}
