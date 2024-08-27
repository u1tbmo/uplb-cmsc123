/* DO NOT EDIT THIS FILE */

/* LIST ADT */

typedef struct node_tag
{
	int value;
	struct node_tag *next;
} NODE;

typedef struct list_tag
{
	NODE *head;
	//NODE* tail;
} LIST;

/*
** printStack()
** requirements: none
** results:
	if list is empty, prints "*empty*"
	otherwise, prints the contents of a list
*/
void printStack(LIST *L);

/*
** createNode()
** requirements: an integer data
** results:
	creates an empty node with value `data`
	initializes fields of the structure
	returns the created node
*/
NODE *createNode(int data);

/*
** createStack()
** requirements: none
** results:
	creates an empty list
	initializes `head` field of the structure
	returns the created list
*/
LIST *createStack();

/*
** isEmpty()
** requirements: none
** results:
	returns 1 if the list is empty
	otherwise returns 0
*/
int isEmpty(LIST *L);

/*
** push()
** requirements: a list and a node to be inserted
** results:
	inserts `node` before the current `head` of the list
*/
void push(LIST *L, NODE *node);

/*
** pop()
** requirements: a list that must not be empty
** results:
	deletes the `head` node of the list
	returns the value of the deleted node
*/
int pop(LIST *L);

/*
** hasParenthesis()
** requirements: a string `expr`
** results:
	checks for parenthesis inside brackets using stack operations
	returns 1 if parenthesis is found
	otherwise returns 0
*/
int hasParenthesis(char *expr);