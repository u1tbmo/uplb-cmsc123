// ___  ____    _  _ ____ ___    ____ ___  _ ___    ___ _  _ _ ____    ____ _ _    ____ 
// |  \ |  |    |\ | |  |  |     |___ |  \ |  |      |  |__| | [__     |___ | |    |___ 
// |__/ |__|    | \| |__|  |     |___ |__/ |  |      |  |  | | ___]    |    | |___ |___ 
//

//Structure for the graph
typedef struct graph_tag {
	int **matrix; //holder of the adjacency matrix
	int num_vertices; //holds the vertex count
} GRAPH;

/*
	CREATE GRAPH
	- allocates memory for the graph
	- initializes the members of the structure + arrays
	- returns the newly created graph
*/
GRAPH *createGraph(int vertices);

/*
	INSERT EDGE
	- plots the adjacent vertices in the adjacency matrix
*/
void insertEdge(GRAPH *G, int u, int v);

/*
	CREATE VISITED
	- returns an integer array called visited - allocate and initialize your array properly
	- this will be used in the dfs() function to mark visited nodes
	- 0: visited, 1: not visited yet
*/
int *createVisited(GRAPH *G);

/*
	DFS
	- implements a non-recursive DFS traversal
	- you may use the functions in the stack.h file 
*/
void dfs(GRAPH *G, int start);

/*
	PRINT MATRIX
	- prints the values of the adjacency matrix
	- follow the layout provided in the sample run
*/
void printMatrix(GRAPH *G);

/*
	FREE MATRIX
	- frees the allocated memory for the adjacency matrix
*/
void freeMatrix(GRAPH *G);
