// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
// #define num_threads 27

// int result[num_threads] = {0}; 

/* structure for passing data to threads */
typedef struct	{
	int psize;
	int **grid;
	int *result;
	bool complete;
	bool valid;
	// pthread_mutex_t *complete;
	// pthread_mutex_t *valid;
} parameters;

// return 1 if KO, 0 if OK, -1 if INCOMPLETE
int checkPuzzle(int psize, int **grid, bool *complete, bool *valid)	{
	*valid = true;
	*complete = true;
	//loop to check completeness
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

int readSudokuPuzzle(char *filename, int ***grid)	{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Could not open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	int psize;
	fscanf(fp, "%d", &psize);
	int **agrid = (int **)malloc((psize + 1) * sizeof(int *));
	for (int row = 1; row <= psize; row++) {
		agrid[row] = (int *)malloc((psize + 1) * sizeof(int));
		for (int col = 1; col <= psize; col++) {
		fscanf(fp, "%d", &agrid[row][col]);
		}
	}
	fclose(fp);
	*grid = agrid;
	return psize;
}

// takes puzzle size and grid[][]
// prints the puzzle
void printSudokuPuzzle(int psize, int **grid)	{
	printf("%d\n", psize);
	for (int row = 1; row <= psize; row++) {
		for (int col = 1; col <= psize; col++) {
		printf("%d ", grid[row][col]);
		}
		printf("\n");
	}
	printf("\n");
}

// takes puzzle size and grid[][]
// frees the memory allocated
void deleteSudokuPuzzle(int psize, int **grid)	{
	for (int row = 1; row <= psize; row++) {
		free(grid[row]);
	}
	free(grid);
}

// expects file name of the puzzle as argument in command line
int main(int argc, char **argv)	{
	if (argc != 2) {
		printf("usage: ./sudoku puzzle.txt\n");
		return EXIT_FAILURE;
	}
	
	int **grid = NULL;
	// int sudokuSize = readSudokuPuzzle(argv[1], &grid);
	// bool valid = false;
	// bool complete = false;
	// checkPuzzle(sudokuSize, grid, &complete, &valid);
	printSudokuPuzzle(sudokuSize, grid);
	deleteSudokuPuzzle(sudokuSize, grid);
	return EXIT_SUCCESS;
}

