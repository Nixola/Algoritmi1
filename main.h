/* allocates a n×n matrix. Complexity is O(n²) if a default value is provided,
   O(n) otherwise. */
int **allocSquareMatrix(int side, int defaultValue);

/* frees a n×n matrix created via allocSquareMatrix.
   Complexity is O(n) always.                       */
void freeSquareMatrix(int **matrix, int side);

/* textbook value swap function */
void swap(int ***a, int ***b);

/* copies the content of a matrix to a new one. Complexity is O(n). */
void copy(int **in, int **out, int side);

/* Implementation of the Floyd-Warshall algorithm to find the length of the 
   shortest paths from and to each node, necessary to find the center[s].*/
int **floydWarshall(int **input, int nNodes);

/* Calculates the ecc. of each node by searching for the longest path from each.
   Stores them in a provided array and returns the minimum found.             */
int findEccentricity(int **matrix, int *eccentricity, int nNodes);

/* Given the minimum eccentricity of the graph, finds out what the nodes with
   said eccentricity are. Returns the amount of centers found.             */
int findCenters(int *eccentricity, int *centers, int nNodes, int minEcc);