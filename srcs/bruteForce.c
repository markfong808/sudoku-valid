#include "../sudoku.h"


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
