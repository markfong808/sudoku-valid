#include "../sudoku.h"

int checkValidThreads(parameters d, int print)	{	// return 1 if KO, 0 if OK
	resetDataSquareGridAll(&d);
	fillDataSquareGrid(&d);

	// pthread_t thread1, thread2, thread3;

	// pthread_create(&thread1, NULL, checkRow, (void*)&d);
    // pthread_create(&thread2, NULL, checkCol, (void*)&d);
    // pthread_create(&thread3, NULL, checkSquareGrid, (void*)&d);

	// void* result1, *result2, *result3;

    // pthread_join(thread1, &result1);
    // pthread_join(thread2, &result2);
    // pthread_join(thread3, &result3);

	// int resultValue1 = (int)(intptr_t)result1;
    // int resultValue2 = (int)(intptr_t)result2;
    // int resultValue3 = (int)(intptr_t)result3;

	int resultValue1 = (int)(intptr_t)checkRow(&d);
    int resultValue2 = (int)(intptr_t)checkCol(&d);
    int resultValue3 = (int)(intptr_t)checkSquareGrid(&d);


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

	if (print == TRUE)
		printf("checkValidThreads: OK\n");
	return (0); // DEFAULT : OK
}


