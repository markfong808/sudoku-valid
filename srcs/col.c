#include "../sudoku.h"


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

void	hardCrossCol(parameters *d, int x)	{	// find numbers in col
	for (int row=1; row <= d->psize; row++)	{	// put numbers from col to find
		if (d->grid[row][x] > 0 && isInFind(d->grid[row][x], &*(d->find), d->psize))
			putInFind(d->grid[row][x], &*(d->find), d->psize);
	}
}
