// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// #define num_threads 27

// int result[num_threads] = {0}; 

typedef struct	{
	int psize;
	int **grid;
	// pthread_mutex_t *complete;
	// pthread_mutex_t *valid;
} parameters;

// return 1 if KO, 0 if OK, -1 if INCOMPLETE
int checkPuzzle(int psize, int **grid, bool *complete, bool *valid)	{
	*valid = true;
	*complete = true;

	for(int row = 1; row <= psize; row++) {
		for(int column = 1;  column <= psize; column++){
			if (grid[row][column] == 0 ){
				*complete = false;
				return (-1); // INCOMPLETE
			}
		}
	}
	// function for rows
	// function for columns
	// function for squares
	return (0); // DEFAULT : OK
}

void	initData(char *filename, parameters *d)	{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Could not open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%d", &d->psize);

	d->grid = (int **)malloc((d->psize + 1) * sizeof(int *));

	for (int row = 1; row <= d->psize; row++) {
		d->grid[row] = (int *)malloc((d->psize + 1) * sizeof(int));
		for (int col = 1; col <= d->psize; col++) {
			fscanf(fp, "%d", &d->grid[row][col]);
		}
	}
	fclose(fp);
}

void printDataGrid(parameters *d)	{
	printf("%d\n", d->psize);
	for (int row = 1; row <= d->psize; row++) {
		for (int col = 1; col <= d->psize; col++) {
		printf("%d ", d->grid[row][col]);
		}
		printf("\n");
	}
	printf("\n");
}

void deleteData(parameters *d)	{
	for (int row = 1; row <= d->psize; row++) {
		free(d->grid[row]);
	}
	free(d->grid);
}

int main(int argc, char **argv)	{
	if (argc != 2) {
		printf("usage: ./sudoku puzzle.txt\n");
		return EXIT_FAILURE;
	}
	parameters data;			// create data struct
	initData(argv[1], &data);	// init data struct

	printDataGrid(&data);
	deleteData(&data);
	return EXIT_SUCCESS;
}

