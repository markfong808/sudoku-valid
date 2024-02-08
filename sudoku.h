#ifndef SUDOKO_H

	#define SUDOKO_H
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


	//	init.c

	void	resetFind(parameters *d);
	void	initDataIncompleteGrid(parameters *d);
	int	initData(char *filename, parameters *d);
	void	initDataIPos(parameters *d);

	// row.c

	int	checkSingleRow(parameters *d, int y);
	int	preCheckRow(parameters *d);
	void*	checkRow(void *arg);
	void	hardCrossRow(parameters *d, int y);

	//	col.c
	int	checkSingleCol(parameters *d, int x);
	int	preCheckCol(parameters *d);
	void*	checkCol(void *arg);
	void	hardCrossCol(parameters *d, int x);

	//	square.c
	void	fillDataSquareGrid(parameters *d);
	void*	checkSquareGrid(void *arg);
	void	resetDataSquareGridAll(parameters *d);

	//	utils.c
	int	isInFind(int x, int *find, int size);
	void	putInFind(int x, int *find, int size);
	int	abs(int i);
	unsigned long long	myPow(int base, int exponent);
	int	checkComplete(parameters *d);

	//	print.c
	void printDataGrid(parameters *d, int neg);
	void printDataSquareGrid(parameters *d);
	void printDataIPos(parameters *d);
	void printAllData(parameters *d);

	//	bruteForce.c
	int	findHardCross(parameters *d, int *row, int *col);
	void	hardCross(parameters *d);
	unsigned long long	useBruteForce(parameters *d);
	void	preCheckGrid(parameters *d);
	void	completeGrid(parameters *d);

	//	thread.c
	int checkValidThreads(parameters d, int print);

	//	free.c
	void deleteData(parameters *d);

#endif
