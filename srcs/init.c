#include "../sudoku.h"

void	resetFind(parameters *d)	{
	for (int i=0; i <= d->psize; i++)	{
		d->find[i] = 0;
	}
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
