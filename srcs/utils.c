#include "../sudoku.h"


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

// return the absolute value
int	abs(int i)	{
	return (i > 0) ? i : -i ;
}

unsigned long long	myPow(int base, int exponent)	{
    unsigned long long res = 1;
    for (int i = 0; i < exponent; i++) {
        res *= base;
    }
    return (res);
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
