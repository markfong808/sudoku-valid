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
// #define MAX_WHILE_BRUTE_FORCE 10000000 // security to crash
#define MAX_WHILE_BRUTE_FORCE 18446744073709551614 // security to crash
#define TRUE 1
#define FALSE 0
#define printfRes FALSE

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
	unsigned long long	maxTry;	// Max try for Brute Force
} parameters;

// return the absolute value
int	abs(int i)	{
	return (i > 0) ? i : -i ;
}

void	fillDataSquareGrid(parameters *d)	{	// return 1 if KO, 0 if OK
	if (DEBUG_CHECK)	printf("fillDataSquareGrid : Start\n");
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			d->sGrid[(((col - 1) / d->sSize) + 1)][(((row - 1) / d->sSize) + 1 )] += abs(d->grid[col][row]);
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
			if (d->sGrid[row][col] != d->sum){
				if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", d->sGrid[row][col], d->sum);
				if (printfRes == TRUE)	printf("checkSquareGrid : KO\n");
				// return (1); // KO
				 return (void*)(intptr_t)1;
			}
			if (DEBUG_CHECK)	printf("%d ", d->sGrid[row][col]);
		}
		if (DEBUG_CHECK)	printf("\n");
	}
	if (printfRes == TRUE)	printf("checkSquareGrid : OK\n");
	// return (0); // OK
	return (void*)(intptr_t)0;
}

int	checkSingleRow(parameters *d, int y)	{	// return 1 if KO, 0 if OK
	if (DEBUG_CHECK)	printf("checkSingleRow\n");
	int	sum = 0;

	for (int col=1; col <= d->psize; col++)	{
		sum += abs(d->grid[y][col]);
		if (DEBUG_CHECK)	printf("%d ", d->grid[y][col]);
	}
	if (DEBUG_CHECK)	printf("\n");
	if (sum != d->sum){
		if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", sum, d->sum);
		if (printfRes == TRUE)	printf("checkSingleRow : KO\n");
		return (1); // KO
	}
	if (printfRes == TRUE)	printf("checkSingleRow : OK\n");
	return (0); // OK
}

void*	checkRow(void *arg)	{	// return 1 if KO, 0 if OK
	parameters *d = arg;
	if (DEBUG_CHECK)	printf("checkRow\n");
	int	sum = 0;
	for (int row=1;  row <= d->psize; row++)	{
		for (int col=1; col <= d->psize; col++)	{
			sum += abs(d->grid[row][col]);
			if (DEBUG_CHECK)	printf("%d ", d->grid[row][col]);
		}
		if (DEBUG_CHECK)	printf("\n");
		if (sum != d->sum){
			if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", sum, d->sum);
			if (printfRes == TRUE)	printf("Rows : KO\n");
			// return (1); // KO
			return (void*)(intptr_t)1;
		}
		sum = 0;
	}
	if (printfRes == TRUE)	printf("Rows : OK\n");
	// return (0); // OK
	return (void*)(intptr_t)0;
}

int	checkSingleCol(parameters *d, int x)	{	// return 1 if KO, 0 if OK
	if (DEBUG_CHECK)	printf("checkSingleCol\n");
	int	sum = 0;

	for (int row=1; row <= d->psize; row++)	{
		sum += abs(d->grid[row][x]);
		if (DEBUG_CHECK)	printf("%d ", d->grid[row][x]);
	}
	if (DEBUG_CHECK)	printf("\n");
	if (sum != d->sum){
		if (DEBUG_CHECK)	printf("sum %d d->sum %d\n", sum, d->sum);
		if (printfRes == TRUE)	printf("checkSingleCol : KO\n");
		return (1); // KO
	}
	if (printfRes == TRUE)	printf("checkSingleCol : OK\n");
	return (0); // OK
}

void*	checkCol(void *arg)	{	// return 1 if KO, 0 if OK
	parameters *d = arg;
	if (DEBUG_CHECK)	printf("checkCol\n");
	int	sum = 0;
	for (int col=1;  col <= d->psize; col++)	{
		for (int row=1; row <= d->psize; row++)	{
			sum += abs(d->grid[row][col]);
			if (DEBUG_CHECK)	printf("%d ", d->grid[row][col]);
		}
		if (DEBUG_CHECK)	printf("\n");
		if (sum != d->sum){
			if (DEBUG_CHECK)	printf ("sum %d d->sum %d\n", sum, d->sum);
			if (printfRes == TRUE)	printf("col : KO\n");
			// return (1); // KO
			return (void*)(intptr_t)1;
		}
		sum = 0;
	}
	if (printfRes == TRUE)	printf("col : OK\n");
	// return (0); // OK
	return (void*)(intptr_t)0;
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
				if (abs(d->grid[row][col]) >= 10 && abs(d->grid[row][col]) <= 35)
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
			printf("%d ", d->sGrid[row][col]);
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

int checkValidThreads(parameters d, int print)	{	// return 1 if KO, 0 if OK
	resetDataSquareGrid(&d);
	fillDataSquareGrid(&d);

	pthread_t thread1, thread2, thread3;

	pthread_create(&thread1, NULL, checkRow, (void*)&d);
    pthread_create(&thread2, NULL, checkCol, (void*)&d);
    pthread_create(&thread3, NULL, checkSquareGrid, (void*)&d);

	void* result1, *result2, *result3;
    pthread_join(thread1, &result1);
    pthread_join(thread2, &result2);
    pthread_join(thread3, &result3);

	int resultValue1 = (int)(intptr_t)result1;
    int resultValue2 = (int)(intptr_t)result2;
    int resultValue3 = (int)(intptr_t)result3;

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
			char tempChar;
			fscanf(fp, "%*[^0-9A-Za-z]%c", &tempChar);

			if (tempChar >= '0' && tempChar <= '9') {
				d->grid[row][col] = tempChar - '0';
			} else if (tempChar >= 'A' && tempChar <= 'Z') {
				d->grid[row][col] = tempChar - 'A' + 10;
			}
		}
		// printf("\n");
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
	if (DEBUG_BRUTE_FORCE)	printf("initDataIPos : Start\n");
	int x = 0;
	d->iPos = (int **)malloc((d->incNbr) * sizeof(int *));	// malloc iPos
	for (int i=0; i < d->incNbr; i++)
		d->iPos[i] = (int *)malloc((3) * sizeof(int));

	/*	Fill iPos	*/
	for (int row = 1; row <= d->psize; row++)	{
		for (int col = 1; col <= d->psize; col++){
			if (d->grid[row][col] < 0)	{
				// if (DEBUG_BRUTE_FORCE)	printf("col: %d, row: %d\n", col, row);
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

int	myPow(int base, int exponent)	{
    int res = 1;
    for (int i = 0; i < exponent; i++) {
        res *= base;
    }
    return (res);
}

void	completeGrid(parameters *d)	{
	unsigned long long i = 0;
	initDataIncompleteGrid(&*d);
	d->maxTry = myPow(d->psize, d->incNbr) + 1;
	printf("completeGrid: incNbr: %d, MaxTry: %llu\n", d->incNbr, d->maxTry);
	// printDataGrid(&*d, TRUE);
	initDataIPos(&*d);
	// printDataIPos(&*d);
	while (i < ULLONG_MAX && i <= d->maxTry && checkValidThreads(*d, FALSE))	{
		i += useBruteForce(&*d);
		printf("%llu.%02llu%% done\r", i * 100 / d->maxTry, (i * 10000 / d->maxTry) % 100);
        fflush(stdout);
		i++;
	}
	printf("%llu.%02llu%% done\n", i * 100 / d->maxTry, (i * 10000 / d->maxTry) % 100);
	printf("completeGrid: find with %llu tests\n", i);
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
	// checkValidNormal(data);
	checkValidThreads(data, TRUE);
	// checkValidNormal(data);
	printDataGrid(&data, FALSE);
	deleteData(&data);
	return EXIT_SUCCESS;
}

