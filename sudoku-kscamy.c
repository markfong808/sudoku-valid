// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdint.h>
#define	DEBUG_CHECK 0
#define	DEBUG_COMPLETE 0
#define	DEBUG_BRUTE_FORCE 0
#define MAX_WHILE 500 // security to avoid abort
#define MAX_WHILE_BRUTE_FORCE 18446744073709551614 // security to crash
#define TRUE 1
#define FALSE 0
#define printfRes FALSE

typedef struct	{
	int	psize;					// The number in a row / col / square
	int	sum;					// The SUM number
	int prod;					// The multiplication product
	int sSize;					// The size of a square
	int	**grid;					// The mainGrid
	int	**sGridSum;				// The squareGridSum, like for 4x4 you have 2x2 squares
	int	**sGridProd;			// The squareGridSum, like for 4x4 you have 2x2 squares
	int	incNbr;					// The number of incomplete points
	int	**iPos;					// The positions of incomplete points
	int	*find;					// The numbers finds, for some operations in hardCross
	unsigned long long	maxTry;	// The Max try for Brute Force
} parameters;

// return the absolute value
int	abs(int i)	{
	return (i > 0) ? i : -i ;
}

void	fillDataSquareGrid(parameters *d)	{	// return 1 if KO, 0 if OK
	if (DEBUG_CHECK)	printf("fillDataSquareGrid : Start\n");
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			d->sGridSum[(((col - 1) / d->sSize) + 1)][(((row - 1) / d->sSize) + 1 )] += abs(d->grid[col][row]);
			d->sGridProd[(((col - 1) / d->sSize) + 1)][(((row - 1) / d->sSize) + 1 )] *= abs(d->grid[col][row]);
			if (DEBUG_CHECK)	printf("col: %d, row: %d, col/: %d, row/: %d\n", col, row, (((col - 1) / d->sSize) + 1), (((row - 1) / d->sSize) + 1 ));
		}
	}
	if (DEBUG_CHECK)	printf("fillDataSquareGrid : OK\n");
}

void*	checkSquareGrid(void *arg)	{	// return 1 if KO, 0 if OK
	parameters *d = arg;
	if (DEBUG_CHECK)	printf("checkSquareGrid\n");
	for (int row=1;  row <= d->sSize; row++)	{
		for (int col=1; col <= d->sSize; col++)	{
			if (d->sGridSum[row][col] != d->sum){
				if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", d->sGridSum[row][col], d->sum);
				if (printfRes == TRUE)	printf("checkSquareGrid : KO\n");
				 return (void*)(intptr_t)1;
			}
			if (d->sGridProd[row][col] != d->prod){
				if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", d->sGridProd[row][col], d->prod);
				if (printfRes == TRUE)	printf("checkSquareGrid : KO\n");
				 return (void*)(intptr_t)1;
			}
			if (DEBUG_CHECK)	printf("%d ", d->sGridSum[row][col]);
		}
		if (DEBUG_CHECK)	printf("\n");
	}
	if (printfRes == TRUE)	printf("checkSquareGrid : OK\n");
	return (void*)(intptr_t)0;
}

int	checkSingleRow(parameters *d, int y)	{	// return 1 if KO, 0 if OK
	if (DEBUG_CHECK)	printf("checkSingleRow:\n");
	int	sum = 0;
	int	prod = 1;

	for (int col=1; col <= d->psize; col++)	{
		sum += abs(d->grid[y][col]);
		prod *= abs(d->grid[y][col]);
		if (DEBUG_CHECK)	printf("%d ", d->grid[y][col]);
	}
	if (DEBUG_CHECK)	printf("\n");
	if (sum != d->sum)	{
		if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", sum, d->sum);
		if (printfRes == TRUE)	printf("checkSingleRow: KO\n");
		return (1); // KO
	}
	if (prod != d->prod)	{
		if (DEBUG_CHECK)	printf ("prod %d d->prod %d\n", prod, d->prod);
		if (printfRes == TRUE)	printf("checkSingleRow: KO\n");
		return (1); // KO
	}

	if (printfRes == TRUE)	printf("checkSingleRow: OK\n");
	return (0); // OK
}

int	preCheckRow(parameters *d)	{	// return 1 if KO, 0 if OK
	int	sum = 0;
	int x = 0;
	int nbr = 0;

	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			sum += abs(d->grid[row][col]);
			if (d->grid[row][col] <= 0)	{
				x = col;
				nbr++;
			}
		}
		if (sum != d->sum && nbr == 1)	{
			// printf("preCheckRow: [%d,%d]:%d\n", row, x, d->sum - sum);
			d->grid[row][x] = d->sum - sum;
			return (1);
		}
		x = 0;
		nbr = 0;
		sum = 0;
	}
	return (0);
}


void*	checkRow(void *arg)	{	// return 1 if KO, 0 if OK
	parameters *d = arg;
	if (DEBUG_CHECK)	printf("checkRow:\n");
	int	sum = 0;
	int prod = 1;

	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			sum += abs(d->grid[row][col]);
			prod *= abs(d->grid[row][col]);
			if (DEBUG_CHECK)	printf("%d ", d->grid[row][col]);
		}
		if (DEBUG_CHECK)	printf("\n");
		if (sum != d->sum){
			if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", sum, d->sum);
			if (printfRes == TRUE)	printf("checkRow: KO\n");
			// return (1); // KO
			return (void*)(intptr_t)1;
		}
		if (prod != d->prod)	{
			if (DEBUG_CHECK)	printf ("prod %d d->prod %d\n", prod, d->prod);
			if (printfRes == TRUE)	printf("checkRow: KO\n");
			return (void*)(intptr_t)1;
		}
		sum = 0;
		prod = 1;
	}
	if (printfRes == TRUE)	printf("checkRow: OK\n");
	// return (0); // OK
	return (void*)(intptr_t)0;
}

int	checkSingleCol(parameters *d, int x)	{	// return 1 if KO, 0 if OK
	if (DEBUG_CHECK)	printf("checkSingleCol:\n");
	int	sum = 0;
	int prod = 1;

	for (int row=1; row <= d->psize; row++)	{
		sum += abs(d->grid[row][x]);
		prod *= abs(d->grid[row][x]);
		if (DEBUG_CHECK)	printf("%d ", d->grid[row][x]);
	}
	if (DEBUG_CHECK)	printf("\n");
	if (sum != d->sum){
		if (DEBUG_CHECK)	printf("sum %d d->sum %d\n", sum, d->sum);
		if (printfRes == TRUE)	printf("checkSingleCol: KO\n");
		return (1); // KO
	}
	if (prod != d->prod)	{
		if (DEBUG_CHECK)	printf ("prod %d d->prod %d\n", prod, d->prod);
		if (printfRes == TRUE)	printf("checkSingleCol: KO\n");
		return (1); // KO
	}

	if (printfRes == TRUE)	printf("checkSingleCol: OK\n");
	return (0); // OK
}

int	preCheckCol(parameters *d)	{	// return 1 if KO, 0 if OK
	int	sum = 0;
	int y = 0;
	int nbr = 0;

	for (int col=1;  col <= d->psize; col++)	{
		for (int row=1; row <= d->psize; row++)	{
			sum += abs(d->grid[row][col]);
			if (d->grid[row][col] <= 0)	{
				y = row;
				nbr++;
			}
		}
		if (sum != d->sum && nbr == 1)	{
			d->grid[y][col] = d->sum - sum;
			return (1);
		}
		y = 0;
		nbr = 0;
		sum = 0;
	}
	return (0);
}

void*	checkCol(void *arg)	{	// return 1 if KO, 0 if OK
	parameters *d = arg;
	if (DEBUG_CHECK)	printf("checkCol:\n");
	int	sum = 0;
	int prod = 1;
	for (int col=1;  col <= d->psize; col++)	{
		for (int row=1; row <= d->psize; row++)	{
			sum += abs(d->grid[row][col]);
			prod *= abs(d->grid[row][col]);
			if (DEBUG_CHECK)	printf("%d ", d->grid[row][col]);
		}
		if (DEBUG_CHECK)	printf("\n");
		if (sum != d->sum){
			if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", sum, d->sum);
			if (printfRes == TRUE)	printf("checkCol: KO\n");
			return (void*)(intptr_t)(1);
		}
		if (prod != d->prod)	{
			if (DEBUG_CHECK)	printf ("prod %d d->prod %d\n", prod, d->prod);
			if (printfRes == TRUE)	printf("checkCol: KO\n");
			return (void*)(intptr_t)1;
		}
		prod = 1;
		sum = 0;
	}
	if (printfRes == TRUE)	printf("checkCol: OK\n");
	return (void*)(intptr_t)(0);
}

int	checkComplete(parameters *d)	{	// return 1 if KO, 0 if OK
	if (DEBUG_CHECK)	printf("checkRow\n");
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			if (DEBUG_CHECK)	printf("%d", d->grid[row][col]);
			if (d->grid[row][col] == 0){
				if (DEBUG_CHECK)	printf ("row %d col %d\n", row, col);
				printf("Complete : KO\n");
				return (1); // KO
			}
		}
		if (DEBUG_CHECK)	printf("\n");
	}
	printf("Complete : OK\n");
	return (0); // OK
}

void printDataGrid(parameters *d, int neg)	{	// set neg to TRUE or FALSE if you whant to display negatives numbers
	printf("Grid :\n");
	for (int row = 1; row <= d->psize; row++)	{
		for (int col = 1; col <= d->psize; col++)	{
			if (neg == FALSE)	{
				if (d->grid[row][col] == 0)
					printf("<%d>", abs(d->grid[row][col]));
				else if (abs(d->grid[row][col]) >= 10 && abs(d->grid[row][col]) <= 35)
					printf(" %c ", 'A' + (abs(d->grid[row][col]) - 10));
				else
					printf(" %d ", abs(d->grid[row][col]));
			}
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
			printf("%d ", d->sGridSum[row][col]);
		printf("\n");
	}
	printf("\n");
}

void printDataIPos(parameters *d)	{
	printf("Incomplete positions :\n");
	for (int i = 0; i < d->incNbr; i++)	{
		printf("%d - [%d;%d] = %d\n", i+1, d->iPos[i][1], d->iPos[i][2], d->grid[d->iPos[i][2]][d->iPos[i][1]]);
	}
	printf("\n");
}

void printAllData(parameters *d)	{
	// printf("d->psize : %d\n", d->psize);
	// printf("d->sum : %d\n", d->sum);
	// printf("d->sSize : %d\n", d->sSize);
	// printf("\n");
	printDataGrid(&*d, FALSE);
	// printDataSquareGrid(&*d);
}

void	resetDataSquareGridAll(parameters *d)	{
	for (int row = 1; row <= d->sSize; row++)	{
		for (int col = 1; col <= d->sSize; col++)	{
			d->sGridSum[row][col] = 0;
			d->sGridProd[row][col] = 1;
		}
	}
}

int checkValidThreads(parameters d, int print)	{	// return 1 if KO, 0 if OK
	resetDataSquareGridAll(&d);
	fillDataSquareGrid(&d);

	// pthread_t thread1, thread2, thread3;

	// pthread_create(&thread1, NULL, checkRow, (void*)&d);
    // pthread_create(&thread2, NULL, checkCol, (void*)&d);
    // pthread_create(&thread3, NULL, checkSquareGrid, (void*)&d);

	// void* result1, *result2, *result3;

    // pthread_join(thread1, &result1);
    // pthread_join(thread2, &result2);
    // pthread_join(thread3, &result3);

	// int resultValue1 = (int)(intptr_t)result1;
    // int resultValue2 = (int)(intptr_t)result2;
    // int resultValue3 = (int)(intptr_t)result3;

	int resultValue1 = (int)(intptr_t)checkRow(&d);
    int resultValue2 = (int)(intptr_t)checkCol(&d);
    int resultValue3 = (int)(intptr_t)checkSquareGrid(&d);


	if (print == TRUE && resultValue1 == 0)
		printf("checkValidThreads: checkRow: OK\n");
	else if (print == TRUE)
		printf("checkValidThreads: checkRow: KO\n");
	if (print == TRUE && resultValue2 == 0)
		printf("checkValidThreads: checkCol: OK\n");
	else if (print == TRUE)
		printf("checkValidThreads: checkCol: KO\n");
	if (print == TRUE && resultValue3 == 0)
		printf("checkValidThreads: checkSquareGrid: OK\n");
	else if (print == TRUE)
		printf("checkValidThreads: checkSquareGrid: KO\n");

	if (resultValue1 == 1)
		return (1);
	if (resultValue2 == 1)
		return (1);
	if (resultValue3 == 1)
		return (1);

	if (print == TRUE)
		printf("checkValidThreads: OK\n");
	return (0); // DEFAULT : OK
}

void	initDataIncompleteGrid(parameters *d)	{
	if (DEBUG_COMPLETE)	printf("initDataIncompleteGrid : Start\n");
	for (int row = 1; row <= d->psize; row++)	{
		for (int col = 1; col <= d->psize; col++){
			if (d->grid[row][col] == 0)	{
				d->grid[row][col] = -1;
				d->incNbr++;
			}
			if (d->grid[row][col] >= 0)	{
				if (DEBUG_COMPLETE)	printf(" %d ", d->grid[row][col]);	}
			else	{
				if (DEBUG_COMPLETE)	printf("%d ", d->grid[row][col]);	}
		}
		if (DEBUG_COMPLETE)	printf("\n");
	}
	if (DEBUG_COMPLETE)	printf("initDataIncompleteGrid : OK\n");
}

void	resetFind(parameters *d)	{
	for (int i=0; i <= d->psize; i++)	{
		d->find[i] = 0;
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

	/*	set prod	*/
	d->prod = 1;
	for (int i=0; i < d->psize; i++)	{
		d->prod *= d->psize - i;
	}

	/*	set grid	*/
	d->grid = (int **)malloc((d->psize + 1) * sizeof(int *));	// malloc grid
	for (int row = 1; row <= d->psize; row++) {				// init grid
		d->grid[row] = (int *)malloc((d->psize + 1) * sizeof(int));
		for (int col = 1; col <= d->psize; col++) {
			char tempChar;
			fscanf(fp, "%*[^0-9A-Za-z]%c", &tempChar);
			if (tempChar >= '0' && tempChar <= '9') {
				d->grid[row][col] = tempChar - '0';
			} else if (tempChar >= 'A' && tempChar <= 'Z') {
				d->grid[row][col] = tempChar - 'A' + 10;
			}
		}
	}
	fclose(fp);

	/*	set sGridSum : squareGrid	*/
	d->sGridSum = (int **)malloc((d->sSize + 1) * sizeof(int *));	// malloc grid
	for (int row = 1; row <= d->sSize; row++) {				// init grid
		d->sGridSum[row] = (int *)malloc((d->sSize + 1) * sizeof(int));
		for (int col = 1; col <= d->sSize; col++)
			d->sGridSum[row][col] = 0;
	}

	/*	set sGridprod : squareGrid	*/
	d->sGridProd = (int **)malloc((d->sSize + 1) * sizeof(int *));	// malloc grid
	for (int row = 1; row <= d->sSize; row++) {				// init grid
		d->sGridProd[row] = (int *)malloc((d->sSize + 1) * sizeof(int));
		for (int col = 1; col <= d->sSize; col++)
			d->sGridProd[row][col] = 1;
	}

	/*	set incNbr : incomplete numbers	*/
	d->incNbr = 0;

	/*	set maxTry	*/
	d->maxTry = 0;

	/*	set find	*/
	d->find = (int*)malloc((d->psize + 1) * sizeof(int));
	resetFind(&*d);
	return (0);
}

void deleteData(parameters *d)	{
	for (int row = 1; row <= d->psize; row++)		// grid
		free(d->grid[row]);
	free(d->grid);
	for (int row = 1; row <= d->sSize; row++)		// sGridSum
		free(d->sGridSum[row]);
	free(d->sGridSum);
	for (int row = 1; row <= d->sSize; row++)		// sGridProd
		free(d->sGridProd[row]);
	free(d->sGridProd);
	free(d->find);
}



void	initDataIPos(parameters *d)	{
	if (DEBUG_BRUTE_FORCE)	printf("initDataIPos : Start\n");
	int x = 0;
	d->iPos = (int **)malloc((d->incNbr) * sizeof(int *));	// malloc iPos
	for (int i=0; i < d->incNbr; i++)	{
		d->iPos[i] = (int *)malloc((4 + d->psize) * sizeof(int));
		for (int u=0; u < (d->psize + 4); u++)
			d->iPos[i][u] = 0;
	}
	// /*	Fill iPos	*/
	for (int row = 1; row <= d->psize; row++)	{
		for (int col = 1; col <= d->psize; col++){
			if (d->grid[row][col] < 0)	{
				if (DEBUG_BRUTE_FORCE)	printf("col: %d, row: %d\n", col, row);
				d->iPos[x][1] = col;	// like x 
				d->iPos[x][2] = row;	// like y
				if (DEBUG_BRUTE_FORCE)	printf("%d - [%d;%d] & [%d;%d]\n", x+1, col, row, d->iPos[x][1], d->iPos[x][2]);
				x++;
			}
		}
	}
	if (DEBUG_BRUTE_FORCE)	printf("initDataIPos : OK\n");
}


unsigned long long	useBruteForce(parameters *d)	{
	unsigned long long ret = 0;
	int i = d->incNbr - 1;
	if (DEBUG_BRUTE_FORCE)	printf("useBruteForce : Start\n");
	while (i >= 0)	{
		if (DEBUG_BRUTE_FORCE)	printf("%d - [%d;%d] = %d\n", i+1, d->iPos[i][1], d->iPos[i][2], d->grid[d->iPos[i][2]][d->iPos[i][1]]);
		if (d->grid[d->iPos[i][2]][d->iPos[i][1]] == d->psize * -1)	{
			d->grid[d->iPos[i][2]][d->iPos[i][1]] = -1;
		}
		else	{
			d->grid[d->iPos[i][2]][d->iPos[i][1]] = d->grid[d->iPos[i][2]][d->iPos[i][1]] - 1;
			/*	opti start	*/
			if (!checkSingleRow(&*d, d->iPos[i][2]) && !checkSingleCol(&*d, d->iPos[i][1]))
				return (ret);
			else	{
				i = d->incNbr;
				ret++;
			}
			/*	opti end	*/
			/*	not opti start	*/
			// return;
			/*	not opti end	*/
		}
		i--;
	}
	if (DEBUG_BRUTE_FORCE)	printf("useBruteForce : OK\n");
	return (ret);
}

unsigned long long	myPow(int base, int exponent)	{
    unsigned long long res = 1;
    for (int i = 0; i < exponent; i++) {
        res *= base;
    }
    return (res);
}

void	preCheckGrid(parameters *d)	{	// return 1 if KO, 0 if OK
	int exit = 3;
	int i=0;
	while (i < 50 && exit > 0)	{
		exit += preCheckRow(&*d);
		exit += preCheckCol(&*d);
		exit--;
		i++;
	}
}

// return 1 if found, 0 if not
int	findHardCross(parameters *d, int *row, int *col)	{	// tell if it is incomplete
	while (*row <= d->psize)	{
		while (*col <= d->psize)	{
			if (d->grid[*row][*col] <= 0)
				return (1);
			*col = *col + 1;
		}
		*col = 1;
		*row = *row + 1;
	}
	return (0);
}

// return 1 if not found, 0 if found
int	isInFind(int x, int *find, int size)	{
	for (int i = 0; i < size; i++)	{
		if (find[i] == x)
			return (0);
	}
	return (1);
}

void	putInFind(int x, int *find, int size)	{	// put x in find a the first free place
	int i = 0;
	while (i < size && find[i] != 0)
		i++;
	find[i] = x;
}

void	hardCrossRow(parameters *d, int y)	{	// find numbers in row
	for (int col=1; col <= d->psize; col++)	{	// put numbers from row to find
		if (d->grid[y][col] > 0 && isInFind(d->grid[y][col], &*(d->find), d->psize))
			putInFind(d->grid[y][col], &*(d->find), d->psize);
	}
}

void	hardCrossCol(parameters *d, int x)	{	// find numbers in col
	for (int row=1; row <= d->psize; row++)	{	// put numbers from col to find
		if (d->grid[row][x] > 0 && isInFind(d->grid[row][x], &*(d->find), d->psize))
			putInFind(d->grid[row][x], &*(d->find), d->psize);
	}
}

void	hardCross(parameters *d)	{	
	printf("hardCross: Start\n");
	int row = 1;
	int col = 1;
	int max = 50;
	int sFind = 0;
	int i = 0;
	while (max > 0 && row <= d->psize && col <= d->psize)	{
		if (findHardCross(&*d, &row, &col)){		// tell if it is incomplete
			hardCrossRow(&*d, row);
			hardCrossCol(&*d, col);
			while (i < d->psize)	{
				sFind += d->find[i];
				i++;
			}
			if (i == d->psize - 1)
				d->grid[row][col] = d->sum - sFind;
			sFind = 0;
			i = 0;
			resetFind(&*d);
		}
		max--;
	}
	printf("hardCross: Ok\n");
}


void	completeGrid(parameters *d)	{
	unsigned long long i = 0;

	preCheckGrid(&*d);
	if (!(checkValidThreads(*d, FALSE)))	{
		printf("completeGrid: preCheckGrid: OK\n");
		return ;
	}
	printf("completeGrid: preCheckGrid: KO\n");
	hardCross(&*d);
	if (!(checkValidThreads(*d, FALSE)))	{
		printf("completeGrid: hardCross: OK\n");
		return ;
	}
	printf("completeGrid: preChehardCrossckGrid: KO\n");
	printf("completeGrid: Launch Brute Force\n");
	initDataIncompleteGrid(&*d);
	d->maxTry = myPow(d->psize, d->incNbr);
	printf("completeGrid: incNbr: %d, MaxTry: %llu\n", d->incNbr, d->maxTry);
	initDataIPos(&*d);
	while (i < ULLONG_MAX && i < d->maxTry && checkValidThreads(*d, FALSE))	{
		i += useBruteForce(&*d);
		printf("%llu.%02llu%% tests done\r", i * 100 / d->maxTry, (i * 10000 / d->maxTry) % 100);
        fflush(stdout);
		i++;
	}
	for (int row = 1; row < d->incNbr; row++)		// iPos
		free(d->iPos[row]);
	free(d->iPos);

	printf("%llu.%02llu%% tests done\n", i * 100 / d->maxTry, (i * 10000 / d->maxTry) % 100);
	printf("completeGrid: find with %llu/%llu tests\n", i, d->maxTry);
}

int main(int argc, char **argv)	{
	if (argc != 2) {
		printf("usage: ./sudoku puzzle.txt\n");
		return EXIT_FAILURE;
	}
	parameters data;			// create data struct
	initData(argv[1], &data);	// init data struct
	printf("Grid at the Start:\n");
	printAllData(&data);

	if (checkComplete(&data))
		completeGrid(&data);
	checkValidThreads(data, TRUE);
	printDataGrid(&data, FALSE);
	deleteData(&data);
	return EXIT_SUCCESS;
}

