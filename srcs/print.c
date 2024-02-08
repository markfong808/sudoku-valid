#include "../sudoku.h"


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
