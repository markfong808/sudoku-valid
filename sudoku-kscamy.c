// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define	debug_fct 0
// #define num_threads 27

// int result[num_threads] = {0}; 

typedef struct	{
	int	psize;
	int	key;	// the SUM number !
	int	**grid;
	// pthread_mutex_t *complete;
	// pthread_mutex_t *valid;
} parameters;

// return 1 if KO, 0 if OK
int	checkRow(parameters *d)	{
	(debug_fct ? printf("checkRow\n") : printf(""));
	int	sum = 0;
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			sum += d->grid[row][col];
			(debug_fct ? printf("%d", d->grid[row][col]) : printf(""));
		}
		(debug_fct ? printf("\n") : printf(""));
		if (sum != d->key){
			(debug_fct ? printf ("sum %d d->key %d\n", sum, d->key) : printf(""));
			printf("Rows : KO\n");
			return (1); // KO
		}
		sum = 0;
	}
	printf("Rows : OK\n");
	return (0); // OK
}

// return 1 if KO, 0 if OK
int	checkCol(parameters *d)	{
	(debug_fct ? printf("checkCol\n") : printf(""));
	int	sum = 0;
	for (int col=1;  col <= d->psize; col++)	{
		for (int row=1; row <= d->psize; row++)	{
			sum += d->grid[row][col];
			(debug_fct ? printf("%d", d->grid[row][col]) : printf(""));
		}
		(debug_fct ? printf("\n") : printf(""));
		if (sum != d->key){
			(debug_fct ? printf ("sum %d d->key %d\n", sum, d->key) : printf(""));
			printf("col : KO\n");
			return (1); // KO
		}
		sum = 0;
	}
	printf("col : OK\n");
	return (0); // OK
}

int	checkComplete(parameters *d)	{
	(debug_fct ? printf("checkRow\n") : printf(""));
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			(debug_fct ? printf("%d", d->grid[row][col]) : printf(""));
			if (d->grid[row][col] == 0){
				(debug_fct ? printf ("row %d col %d\n", row, col) : printf(""));
				printf("Complete : KO\n");
				return (1); // KO
			}
		}
		(debug_fct ? printf("\n") : printf(""));
	}
	printf("Complete : OK\n");
	return (0); // OK
}

// return 1 if KO, 0 if OK, -1 if INCOMPLETE
int checkValid(parameters d)	{
	// function for rows
	checkRow(&d);
	// function for columns
	checkCol(&d);
	// function for squares
	return (0); // DEFAULT : OK
}

void	initData(char *filename, parameters *d)	{
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Could not open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%d", &d->psize);							//	init psize

	d->key = (d->psize * (d->psize + 1) ) / 2;				//	init key

	d->grid = (int **)malloc((d->psize + 1) * sizeof(int *));	// malloc grid

	for (int row = 1; row <= d->psize; row++) {				// init grid
		d->grid[row] = (int *)malloc((d->psize + 1) * sizeof(int));
		for (int col = 1; col <= d->psize; col++) {
			fscanf(fp, "%d", &d->grid[row][col]);
		}
	}
	fclose(fp);
}

void printDataGrid(parameters *d)	{
	printf("d->psize : %d\n", d->psize);
	printf("d->key : %d\n", d->key);
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

	checkComplete(&data);
	checkValid(data);		

	deleteData(&data);
	return EXIT_SUCCESS;
}

