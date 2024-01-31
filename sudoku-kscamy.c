// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define	DEBUG_CHECK 0
#define	DEBUG_COMPLETE 0
#define	DEBUG_BRUTE_FORCE 0
#define MAX_WHILE 500 // security to avoid abort
#define MAX_WHILE_BRUTE_FORCE 10000000 // security to crash
#define TRUE 1
#define FALSE 0

// #define num_threads 27

// int result[num_threads] = {0}; 

typedef struct	{
	int	psize;
	int	sum;		// the SUM number
	int sSize;		// size of a square
	int	**grid;
	int	**sGrid;	// Square Grid, like for 4x4 you have 2x2 square
	int	incNbr;	// number of incomplete points
	int	**iPos;	// positions of incomplete points
	int	maxTry;	// Max try for Brute Force
} parameters;

// return the absolute value
int	abs(int i)	{
	return (i > 0) ? i : -i ;
}

void	fillDataSquareGrid(parameters *d)	{	// return 1 if KO, 0 if OK
	(DEBUG_CHECK ? printf("fillDataSquareGrid : Start\n") : 0);
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			d->sGrid[(((col - 1) / d->sSize) + 1)][(((row - 1) / d->sSize) + 1 )] += abs(d->grid[col][row]);
			(DEBUG_CHECK ? printf("col: %d, row: %d, col/: %d, row/: %d\n", col, row, (((col - 1) / d->sSize) + 1), (((row - 1) / d->sSize) + 1 )) : 0);
		}
	}
	(DEBUG_CHECK ? printf("fillDataSquareGrid : OK\n") : 0);
}

int	checkSquareGrid(parameters *d)	{	// return 1 if KO, 0 if OK
	(DEBUG_CHECK ? printf("checkSquareGrid\n") : 0);
	for (int row=1;  row <= d->sSize; row++)	{
		for (int col=1; col <= d->sSize; col++)	{
			if (d->sGrid[row][col] != d->sum){
				(DEBUG_CHECK ? printf ("sum %d d->sum %d\n", d->sGrid[row][col], d->sum) : 0);
				printf("checkSquareGrid : KO\n");
				return (1); // KO
			}
			(DEBUG_CHECK ? printf("%d ", d->sGrid[row][col]) : 0);
		}
		(DEBUG_CHECK ? printf("\n") : 0);
	}
	printf("checkSquareGrid : OK\n");
	return (0); // OK
}

int	checkRow(parameters *d, int printfRes)	{	// return 1 if KO, 0 if OK
	(DEBUG_CHECK ? printf("checkRow\n") : 0);
	int	sum = 0;
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			sum += abs(d->grid[row][col]);
			(DEBUG_CHECK ? printf("%d ", d->grid[row][col]) : 0);
		}
		(DEBUG_CHECK ? printf("\n") : 0);
		if (sum != d->sum){
			(DEBUG_CHECK ? printf ("sum %d d->sum %d\n", sum, d->sum) : 0);
			(printfRes == TRUE) ? printf("Rows : KO\n") : 0;
			return (1); // KO
		}
		sum = 0;
	}
	(printfRes == TRUE) ? printf("Rows : OK\n") : 0;
	return (0); // OK
}

int	checkCol(parameters *d, int printfRes)	{	// return 1 if KO, 0 if OK
	(DEBUG_CHECK ? printf("checkCol\n") : 0);
	int	sum = 0;
	for (int col=1;  col <= d->psize; col++)	{
		for (int row=1; row <= d->psize; row++)	{
			sum += abs(d->grid[row][col]);
			(DEBUG_CHECK ? printf("%d ", d->grid[row][col]) : 0);
		}
		(DEBUG_CHECK ? printf("\n") : 0);
		if (sum != d->sum){
			(DEBUG_CHECK ? printf ("sum %d d->sum %d\n", sum, d->sum) : 0);
			(printfRes == TRUE) ? printf("col : KO\n") : 0;
			return (1); // KO
		}
		sum = 0;
	}
	(printfRes == TRUE) ? printf("col : OK\n") : 0;
	return (0); // OK
}

int	checkComplete(parameters *d)	{	// return 1 if KO, 0 if OK
	(DEBUG_CHECK ? printf("checkRow\n") : 0);
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			(DEBUG_CHECK ? printf("%d", d->grid[row][col]) : 0);
			if (d->grid[row][col] == 0){
				(DEBUG_CHECK ? printf ("row %d col %d\n", row, col) : 0);
				printf("Complete : KO\n");
				return (1); // KO
			}
		}
		(DEBUG_CHECK ? printf("\n") : 0);
	}
	printf("Complete : OK\n");
	return (0); // OK
}

void printDataGrid(parameters *d, int neg)	{	// set neg to TRUE or FALSE if you whant to display negatives numbers
	printf("Grid :\n");
	for (int row = 1; row <= d->psize; row++)	{
		for (int col = 1; col <= d->psize; col++)	{
			if (neg == FALSE)
				printf(" %d ", abs(d->grid[row][col]));
			else if (neg == TRUE && d->grid[row][col] >= 0)
				printf(" %d ", d->grid[row][col]);
			else
				printf("%d ", d->grid[row][col]);
		}
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

void printDataIPos(parameters *d)	{
	printf("Incomple positions :\n");
	for (int i = 0; i < d->incNbr; i++)	{
		printf("%d - [%d;%d] = %d\n", i+1, d->iPos[i][1], d->iPos[i][2], d->grid[d->iPos[i][2]][d->iPos[i][1]]);
	}
	printf("\n");
}

void printAllData(parameters *d)	{
	printf("d->psize : %d\n", d->psize);
	printf("d->sum : %d\n", d->sum);
	printf("d->sSize : %d\n", d->sSize);
	printf("\n");
	printDataGrid(&*d, FALSE);
	printDataSquareGrid(&*d);
}

void	resetDataSquareGrid(parameters *d)	{
	for (int row = 1; row <= d->sSize; row++)	{
		for (int col = 1; col <= d->sSize; col++)
			d->sGrid[row][col] = 0;
	}
}

int checkValid(parameters d)	{	// return 1 if KO, 0 if OK
	// function for rows
	if (checkRow(&d, FALSE))
		return (1);
	// function for columns
	if (checkCol(&d, FALSE))
		return (1);
	// function for squares
	resetDataSquareGrid(&d);
	fillDataSquareGrid(&d);
	if (checkSquareGrid(&d))
		return (1);
	return (0); // DEFAULT : OK
}


void	initDataIncompleteGrid(parameters *d)	{
	(DEBUG_COMPLETE ? printf("initDataIncompleteGrid : Start\n") : 0);
	for (int row = 1; row <= d->psize; row++)	{
		for (int col = 1; col <= d->psize; col++){
			if (d->grid[row][col] == 0)	{
				d->grid[row][col] = -1;
				d->incNbr++;
			}
			if (d->grid[row][col] >= 0)
				(DEBUG_COMPLETE ? printf(" %d ", d->grid[row][col]) : 0);
			else
				(DEBUG_COMPLETE ? printf("%d ", d->grid[row][col]) : 0);
			
		}
		(DEBUG_COMPLETE ? printf("\n") : 0);
	}
	(DEBUG_COMPLETE ? printf("initDataIncompleteGrid : OK\n") : 0);
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

	/*	set incNbr : incomplete numbers	*/
	d->incNbr = 0;

	/*	set maxTry	*/
	d->maxTry = 0;
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

void	initDataIPos(parameters *d)	{
	(DEBUG_BRUTE_FORCE ? printf("initDataIPos : Start\n") : 0);
	int x = 0;
	d->iPos = (int **)malloc((d->incNbr) * sizeof(int *));	// malloc iPos
	for (int i=0; i < d->incNbr; i++)
		d->iPos[i] = (int *)malloc((3) * sizeof(int));

	/*	Fill iPos	*/
	for (int row = 1; row <= d->psize; row++)	{
		for (int col = 1; col <= d->psize; col++){
			if (d->grid[row][col] < 0)	{
				// (DEBUG_BRUTE_FORCE ? printf("col: %d, row: %d\n", col, row) : 0);
				d->iPos[x][1] = col;	// like x 
				d->iPos[x][2] = row;	// like y
				(DEBUG_BRUTE_FORCE ? printf("%d - [%d;%d] & [%d;%d]\n", x+1, col, row, d->iPos[x][1], d->iPos[x][2]) : 0);
				x++;
			}
		}
	}
	(DEBUG_BRUTE_FORCE ? printf("initDataIPos : OK\n") : 0);
}


void	useBruteForce(parameters *d)	{
	int i = d->incNbr - 1;
	(DEBUG_BRUTE_FORCE ? printf("useBruteForce : Start\n") : 0);
	while (i >= 0)	{
		(DEBUG_BRUTE_FORCE ? printf("%d - [%d;%d] = %d\n", i+1, d->iPos[i][1], d->iPos[i][2], d->grid[d->iPos[i][2]][d->iPos[i][1]]) : 0);
		if (d->grid[d->iPos[i][2]][d->iPos[i][1]] == d->psize * -1)	{
			d->grid[d->iPos[i][2]][d->iPos[i][1]] = -1;
		}
		else	{
			d->grid[d->iPos[i][2]][d->iPos[i][1]] = d->grid[d->iPos[i][2]][d->iPos[i][1]] - 1;
			return;
		}
		i--;
	}
	(DEBUG_BRUTE_FORCE ? printf("useBruteForce : OK\n") : 0);
}

int	myPow(int base, int exponent)	{
    int res = 1;
    for (int i = 0; i < exponent; i++) {
        res *= base;
    }
    return (res);
}

void	completeGrid(parameters *d)	{
	int i = 0;
	initDataIncompleteGrid(&*d);
	d->maxTry = myPow(d->psize, d->incNbr) + 1;
	printf("completeGrid: incNbr: %d, MaxTry: %d\n", d->incNbr, d->maxTry);
	printDataGrid(&*d, TRUE);
	initDataIPos(&*d);
	printDataIPos(&*d);
	while (i < MAX_WHILE_BRUTE_FORCE && i <= d->maxTry && checkValid(*d))	{
		useBruteForce(&*d);
		i++;
	}
}

int main(int argc, char **argv)	{
	if (argc != 2) {
		printf("usage: ./sudoku puzzle.txt\n");
		return EXIT_FAILURE;
	}
	parameters data;			// create data struct
	initData(argv[1], &data);	// init data struct
	printAllData(&data);

	if (checkComplete(&data))
		completeGrid(&data);
	checkValid(data);
	printDataGrid(&data, FALSE);
	deleteData(&data);
	return EXIT_SUCCESS;
}

