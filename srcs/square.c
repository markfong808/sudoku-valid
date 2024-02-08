#include "../sudoku.h"


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


void	resetDataSquareGridAll(parameters *d)	{
	for (int row = 1; row <= d->sSize; row++)	{
		for (int col = 1; col <= d->sSize; col++)	{
			d->sGridSum[row][col] = 0;
			d->sGridProd[row][col] = 1;
		}
	}
}
