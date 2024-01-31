// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define	debug_fct 0
#define MAX_WHILE 500 // security to avoid abort
// #define num_threads 27

// int result[num_threads] = {0}; 

typedef struct	{
	int	psize;
	int	sum;		// the SUM number
	int sSize;		// size of a square
	int	**grid;
	int	**sGrid;	// Square Grid, like for 4x4 you have 2x2 square
	// pthread_mutex_t *complete;
	// pthread_mutex_t *valid;
} parameters;

// return 1 if KO, 0 if OK
void	fillDataSquareGrid(parameters *d)	{
	(debug_fct ? printf("fillDataSquareGrid : Start\n") : printf(""));
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			d->sGrid[(((col - 1) / d->sSize) + 1)][(((row - 1) / d->sSize) + 1 )] += d->grid[col][row];
			(debug_fct ? printf("col: %d, row: %d, col/: %d, row/: %d\n", col, row, (((col - 1) / d->sSize) + 1), (((row - 1) / d->sSize) + 1 )) : printf(""));
		}
	}
	(debug_fct ? printf("fillDataSquareGrid : OK\n") : printf(""));
}

// return 1 if KO, 0 if OK
int	checkSquare(parameters *d)	{
	(debug_fct ? printf("checkSquare\n") : printf(""));
	for (int row=1;  row <= d->sSize; row++)	{
		for (int col=1; col <= d->sSize; col++)	{
			if (d->sGrid[row][col] != d->sum){
				(debug_fct ? printf ("sum %d d->sum %d\n", d->sGrid[row][col], d->sum) : printf(""));
				printf("checkSquare : KO\n");
				return (1); // KO
			}
			(debug_fct ? printf("%d ", d->sGrid[row][col]) : printf(""));
		}
		(debug_fct ? printf("\n") : printf(""));
	}
	printf("checkSquare : OK\n");
	return (0); // OK
}

// return 1 if KO, 0 if OK
int	checkRow(parameters *d)	{
	(debug_fct ? printf("checkRow\n") : printf(""));
	int	sum = 0;
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			sum += d->grid[row][col];
			(debug_fct ? printf("%d ", d->grid[row][col]) : printf(""));
		}
		(debug_fct ? printf("\n") : printf(""));
		if (sum != d->sum){
			(debug_fct ? printf ("sum %d d->sum %d\n", sum, d->sum) : printf(""));
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
			(debug_fct ? printf("%d ", d->grid[row][col]) : printf(""));
		}
		(debug_fct ? printf("\n") : printf(""));
		if (sum != d->sum){
			(debug_fct ? printf ("sum %d d->sum %d\n", sum, d->sum) : printf(""));
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


void printAllData(parameters *d)	{
	printf("d->psize : %d\n", d->psize);
	printf("d->sum : %d\n", d->sum);
	printf("d->sSize : %d\n", d->sSize);
	printf("\n");
	printf("Grid :\n");
	for (int row = 1; row <= d->psize; row++)	{
		for (int col = 1; col <= d->psize; col++)
			printf("%d ", d->grid[row][col]);
		printf("\n");
	}
	printf("\n");
	printf("Square Grid :\n");
	for (int row = 1; row <= d->sSize; row++)	{
		for (int col = 1; col <= d->sSize; col++)
			printf("%d ", d->sGrid[row][col]);
		printf("\n");
	}
	printf("\n");
}

void printDataSquareGrid(parameters *d)	{
	printf("Square Grid :\n");
	for (int row = 1; row <= d->sSize; row++)	{
		for (int col = 1; col <= d->sSize; col++)
			printf("%d ", d->sGrid[row][col]);
		printf("\n");
	}
	printf("\n");
}


// return 1 if KO, 0 if OK, -1 if INCOMPLETE
int checkValid(parameters d)	{
	// function for rows
	checkRow(&d);
	// function for columns
	checkCol(&d);
	// function for squares
	fillDataSquareGrid(&d);
	// printDataSquareGrid(&d);
	checkSquare(&d);
	return (0); // DEFAULT : OK
}

void	resetDataSquareGrid(parameters *d)	{
	for (int row = 1; row <= d->sSize; row++)	{
		for (int col = 1; col <= d->sSize; col++)
			d->sGrid[row][col] = 0;
	}
}

int	initData(char *filename, parameters *d)	{	// init Data struct
	FILE *fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Could not open file %s\n", filename);
		exit(EXIT_FAILURE);
	}
	fscanf(fp, "%d", &d->psize);							//	init psize

	/*	set sSize : squareSize	*/
	int i = 0;
	while (i <= MAX_WHILE && i < d->psize && i * i != d->psize)
		i++;
	if (i == MAX_WHILE)	{
		printf("Error: in funciton void	initData(char *filename, parameters *d); while go to max !\n");
		return (1);
	}
	else if (i * i == d->psize)
		d->sSize = i;

	/*	set sum	*/
	d->sum = (d->psize * (d->psize + 1) ) / 2;

	/*	set grid	*/
	d->grid = (int **)malloc((d->psize + 1) * sizeof(int *));	// malloc grid
	for (int row = 1; row <= d->psize; row++) {				// init grid
		d->grid[row] = (int *)malloc((d->psize + 1) * sizeof(int));
		for (int col = 1; col <= d->psize; col++) {
			fscanf(fp, "%d", &d->grid[row][col]);
		}
	}
	fclose(fp);

	/*	set sGrid : squareGrid	*/
	d->sGrid = (int **)malloc((d->sSize + 1) * sizeof(int *));	// malloc grid
	for (int row = 1; row <= d->sSize; row++) {				// init grid
		d->sGrid[row] = (int *)malloc((d->sSize + 1) * sizeof(int));
		for (int col = 1; col <= d->sSize; col++)
			d->sGrid[row][col] = 0;
	}
	return (0);
}

void deleteData(parameters *d)	{
	for (int row = 1; row <= d->psize; row++)
		free(d->grid[row]);
	free(d->grid);
	for (int row = 1; row <= d->sSize; row++)
		free(d->sGrid[row]);
	free(d->sGrid);
}

int main(int argc, char **argv)	{
	if (argc != 2) {
		printf("usage: ./sudoku puzzle.txt\n");
		return EXIT_FAILURE;
	}
	parameters data;			// create data struct
	initData(argv[1], &data);	// init data struct
	printAllData(&data);

	checkComplete(&data);
	checkValid(data);

	deleteData(&data);
	return EXIT_SUCCESS;
}

