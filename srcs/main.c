#include "../sudoku.h"



int main(int argc, char **argv)	{
	if (argc != 2) {
		printf("usage: ./sudoku puzzle.txt\n");
		return EXIT_FAILURE;
	}
	parameters data;			// create data struct
	initData(argv[1], &data);	// init data struct
	printf("Grid at the Start:\n");
	printAllData(&data);

	if (checkComplete(&data))
		completeGrid(&data);
	checkValidThreads(data, TRUE);
	printDataGrid(&data, FALSE);
	deleteData(&data);
	return EXIT_SUCCESS;
}

