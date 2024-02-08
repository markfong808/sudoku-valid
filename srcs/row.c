#include "../sudoku.h"


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


void	hardCrossRow(parameters *d, int y)	{	// find numbers in row
	for (int col=1; col <= d->psize; col++)	{	// put numbers from row to find
		if (d->grid[y][col] > 0 && isInFind(d->grid[y][col], &*(d->find), d->psize))
			putInFind(d->grid[y][col], &*(d->find), d->psize);
	}
}

