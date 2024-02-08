#include "../sudoku.h"

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
