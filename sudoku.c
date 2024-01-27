// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define num_threads 27

// int valid[num_threads] = {0};
pthread_t threads[num_threads];

/* structure for passing data to threads */
typedef struct
{
  int row;
  int column;
  int psize;

} parameters;

void *ColumnValid(parameters* par,int **grid)
{
  parameters *parm = (parameters *)par;
  //int row = parm->row;
  int col = parm->column;
  int psize = parm->psize;
  // int **grid = data->grid;

  //check if number 1-9 only appear once in column
  // Dynamic memory allocation for valider array
  int *valider = (int *)malloc(psize * sizeof(int));

  if (valider == NULL)
  {
    // Handle memory allocation failure
    pthread_exit(NULL);
  }

  // Initialize the array with all 0
  for (int i = 0; i < psize; i++)
  {
    valider[i] = 0;
  }
  for (int i = 1; i < psize; i++)
  {
    int num = grid[i][col];
    if (num < 1 || num > psize || valider[num - 1] == 1) {
      pthread_exit(NULL);
    } else {
      valider[num - 1] = 1; //mark it as seen
    }
  }
  //no duplicated found
  pthread_exit(NULL);
}
// takes puzzle size and grid[][] representing sudoku puzzle
// and row booleans to be assigned: complete and valid.
// row-0 and column-0 is ignored for convenience, so a 9x9 puzzle
// has grid[1][1] as the top-left element and grid[9]9] as bottom right
// A puzzle is complete if it can be completed with no 0s in it
// If complete, a puzzle is valid if all rows/columns/boxes have numbers from 1
// to psize For incomplete puzzles, we cannot say anything about validity
void checkPuzzle(int psize, int **grid, bool *complete, bool *valid) {
  // YOUR CODE GOES HERE and in HELPER FUNCTIONS  
  *valid = true;
  *complete = true;
  
  //loop to check completeness
  for(int row = 1; row < psize; row++) {
    for(int column = 1;  column < psize; column++){
      if (grid[row][column] == 0 ){
        *complete = false;
        break;
      }
    }
    if(! *complete){
      break;
    }
  }
  if(*complete){ //if complete is true
    pthread_t columnThread[psize];//keep track column thread identifiers
    
    for(int col = 1; col <= psize; col++){
      parameters *data = (parameters *)malloc(sizeof(parameters));
      data->row = 1;
      data->column = col;
      data->psize = psize;
    
     if(pthread_create(&columnThread[col],NULL,ColumnValid,(void *) data) != 0){
      *valid = false;
      return;
     }
    }
    
    for(int i = 0; i < num_threads; i++){
      pthread_join(threads[i],NULL);
    }
  }
}

// takes filename and pointer to grid[][]
// returns size of Sudoku puzzle and fills grid
int readSudokuPuzzle(char *filename, int ***grid) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  int psize;
  fscanf(fp, "%d", &psize);
  int **agrid = (int **)malloc((psize + 1) * sizeof(int *));
  for (int row = 1; row <= psize; row++) {
    agrid[row] = (int *)malloc((psize + 1) * sizeof(int));
    for (int col = 1; col <= psize; col++) {
      fscanf(fp, "%d", &agrid[row][col]);
    }
  }
  fclose(fp);
  *grid = agrid;
  return psize;
}

// takes puzzle size and grid[][]
// prints the puzzle
void printSudokuPuzzle(int psize, int **grid) {
  printf("%d\n", psize);
  for (int row = 1; row <= psize; row++) {
    for (int col = 1; col <= psize; col++) {
      printf("%d ", grid[row][col]);
    }
    printf("\n");
  }
  printf("\n");
}

// takes puzzle size and grid[][]
// frees the memory allocated
void deleteSudokuPuzzle(int psize, int **grid) {
  for (int row = 1; row <= psize; row++) {
    free(grid[row]);
  }
  free(grid);
}

// expects file name of the puzzle as argument in command line
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: ./sudoku puzzle.txt\n");
    return EXIT_FAILURE;
  }
  //Declare thread object
  // pthread_t columnThreads[num_threads];
  // pthread_t rowThreads[num_threads];

  // pthread_create(&columnThreads,NULL,&,(void *) data);
  
  // grid is a 2D array
  int **grid = NULL;
  // find grid size and fill grid
  int sudokuSize = readSudokuPuzzle(argv[1], &grid);
  bool valid = false;
  bool complete = false;
  checkPuzzle(sudokuSize, grid, &complete, &valid);
  printf("Complete puzzle? ");
  printf(complete ? "true\n" : "false\n");
  if (complete) {
    printf("Valid puzzle? ");
    printf(valid ? "true\n" : "false\n");
  }
  printSudokuPuzzle(sudokuSize, grid);
  deleteSudokuPuzzle(sudokuSize, grid);
  return EXIT_SUCCESS;
}
