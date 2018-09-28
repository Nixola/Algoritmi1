#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#ifndef _CRT_SECURE_NO_WARNING
#define _CRT_SECURE_NO_WARNING
#endif

#define INF INT_MAX
#define minInt(a, b) (a)<(b)?(a):(b)
#define maxInt(a, b) (a)>(b)?(a):(b)

#define benchmark

#ifdef benchmark
#include <time.h>
#define MEASURE(name, func) {clock_t first=clock();func;clock_t second=clock();fprintf(stderr, "Processor time taken by %s: %ldns\n", name, (second-first)*1000*1000/CLOCKS_PER_SEC);}
#else
#define MEASURE(name, func) func
#endif

int** allocSquareMatrix(int side, int defaultValue) {
	int** matrix = malloc(side * sizeof(int*));
	int i, j;
	for (i = 0; i < side; i++) {
		matrix[i] = malloc(side * sizeof(int));
		if (defaultValue) {
			for (j = 0; j < side; j++) {
				matrix[i][j] = defaultValue;
			}
		}
	}
	return matrix;
}

void freeSquareMatrix(int** matrix, int side) {
	int i;
	for (i = 0; i < side; i++) {
		free(matrix[i]);
	}
	free(matrix);
}

void swap(int*** a, int*** b) {
	int** tmp = *a;
	*a = *b;
	*b = tmp;
}

void copy(int** in, int** out, int side) {
	int i, j;
	for (i = 0; i < side; i++) {
		for (j = 0; j < side; j++) {
			out[i][j] = in[i][j];
		}
	}
}

int** floydWarshall(int** input, int nNodes) {
	int k, i, j;
	int** matrix 	= input;
	int** matrixOld	= allocSquareMatrix(nNodes, 0);
	for (k = 0; k < nNodes; k++) {
		swap(&matrix, &matrixOld);
		for (i = 0; i < nNodes; i++) {
			for (j = 0; j < nNodes; j++) {
				int partial = (matrixOld[i][k] == INF || matrixOld[k][j] == INF) ? INF : matrixOld[i][k] + matrixOld[k][j];
				matrix[i][j] = minInt(matrixOld[i][j], partial);
			}
		}
	}
	freeSquareMatrix(matrixOld, nNodes);
	return matrix;
}

int findEccentricity(int** matrix, int* eccentricity, int nNodes) {
	int i, j, minEcc = INF;
	for (i = 0; i < nNodes; i++) {
		int max = matrix[i][0];
		for (j = 1; j < nNodes; j++) {
			max = maxInt(max, matrix[i][j]);
		}
		eccentricity[i] = max;
		minEcc = minInt(minEcc, max);
	}
	return minEcc;
}

int findCenters(int* eccentricity, int* centers, int nNodes, int minEcc) {
	int i, nCenters = 0;
	for (i = 0; i < nNodes; i++) {
		if (eccentricity[i] == minEcc) {
			centers[nCenters++] = i;
		}
	}
	return nCenters;
}

int main(int argC, char** argV) {
	if (argC != 3) {
		printf("Nope\n");
		return -1;
	}
	char* inputFile = argV[1];
	char* outputFile = argV[2];
	int nNodes, nArcs;
	FILE* file;
	int** matrix;
	int* eccentricity;
	int* centers;
	int nCenters;

	if ((file = fopen(inputFile, "r"))) {
		fscanf(file, "%d %d", &nNodes, &nArcs);
	} else {
		printf("Could not open file %s\n", inputFile);
		return -2;
	}

	MEASURE("matrix allocation", matrix = allocSquareMatrix(nNodes, INF));
	
	for (int n = 0; n < nArcs; n++) {
		int i, j, val;
		fscanf(file, "%d %d %d", &i, &j, &val);
		if (val < matrix[--i][--j]) {
			matrix[i][j] = val;
		}
	}

	fclose(file);

	for (int i = 0; i < nNodes; i++) {
		matrix[i][i] = 0;
	}

	MEASURE("Floyd-Warshall algorithm", matrix = floydWarshall(matrix, nNodes));

	MEASURE("eccentricity calculation", eccentricity = malloc(nNodes * sizeof(int)));
	centers = malloc(nNodes * sizeof(int));

	MEASURE("finding centers", nCenters = findCenters(eccentricity, centers, nNodes, findEccentricity(matrix, eccentricity, nNodes)));

	if (!(file = fopen(outputFile, "w"))) {
		fprintf(stderr, "Couldn't open output file \"%s\". Falling back to stdout.\n", outputFile);
		file = stdout;
	}

	for (int i = 0; i < nCenters; i++) {
		fprintf(file, "%d", centers[i] + 1);
		if (i+1 != nCenters) {
			fprintf(file, ", ");
		}
	}
	if (file == stdout) {
		fprintf(file, "\n");
	}
	fclose(file);

	return 0;
}