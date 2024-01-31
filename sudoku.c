// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define num_threads 27

int result[num_threads] = {0};
int x = 0;
/* structure for passing data to threads */
typedef struct
{
  int row;
  int column;
  int psize;
  int **grid;
  bool complete;
  bool valid;
  // pthread_mutex_t *mutex; // Mutex for synchronization

} parameters;

void *RowCheck(void *par)
{
  parameters *parm = (parameters *)par;
  int row = parm->row;
  bool valid = parm->valid;
  int psize = parm->psize;
  int **grid = parm->grid;
  int *valider = (int *)malloc((psize + 1) * sizeof(int));
  for (int i = 0; i <= psize; i++)
  {
    valider[i] = 0;
  }
  for (int j = 1; j <= psize; j++)
  {
    int num = grid[row][j];
    if (num < 1 || num > psize || valider[num] == 1)
    {
      valid = false;
      x = 1;
      break;
    }
    else
    {
      valider[num] = 1;
    }
  }
  // printf("Valid after the loop: %s\n", valid ? "true" : "false");
  //  printf("Valider array after the loop: ");
  //  for (int i = 0; i <= psize; i++)
  //  {
  //    printf("%d ", valider[i]);
  //  }
  //  printf("\n");
  // printf("Valid after the loop: %s\n", valid ? "true" : "false");
  free(valider);

  // printf("Thread result: %d\n" ,valid);
  // return (void *) valid;
  // pthread_exit(NULL);
}

void *ColumnCheck(void *par)
{
  parameters *parm = (parameters *)par;
  int col = parm->column;
  int psize = parm->psize;
  int **grid = parm->grid;
  bool valid = parm->valid;

  int *valider = (int *)malloc((psize + 1) * sizeof(int));

  for (int i = 0; i <= psize; i++)
  {
    valider[i] = 0;
  }
  for (int i = 1; i <= psize; i++)
  {
    int num = grid[i][col];
    if (num < 1 || num > psize || valider[num] == 1)
    {
      valid = false;
      x = 1;
      break;
    }
    else
    {
      valider[num] = 1;
    }
  }
  // printf("Valid after the loop: %s\n", valid ? "true" : "false");
  //  Print the valider array
  printf("Valider array after the loop: ");
  for (int i = 0; i <= psize; i++)
  {
    printf("%d ", valider[i]);
  }
  printf("\n");
  // printf("Valid after the loop: %s\n", valid ? "true" : "false");
  free(valider);
}

void *SquareCheck(void *par)
{
  parameters *parm = (parameters *)par;
  int col = parm->column;
  int psize = parm->psize;
  int **grid = parm->grid;
  bool valid = parm->valid;
}
  // takes puzzle size and grid[][] representing sudoku puzzle
  // and row booleans to be assigned: complete and valid.
  // row-0 and column-0 is ignored for convenience, so a 9x9 puzzle
  // has grid[1][1] as the top-left element and grid[9]9] as bottom right
  // A puzzle is complete if it can be completed with no 0s in it
  // If complete, a puzzle is valid if all rows/columns/boxes have numbers from 1
  // to psize For incomplete puzzles, we cannot say anything about validity
  void checkPuzzle(int psize, int **grid, bool *complete, bool *valid)
  {
    // YOUR CODE GOES HERE and in HELPER FUNCTIONS
    *valid = true;
    *complete = true;
    // loop to check completeness
    for (int row = 1; row <= psize; row++)
    {
      for (int column = 1; column <= psize; column++)
      {
        if (grid[row][column] == 0)
        {
          *complete = false;
          break;
        }
      }
    }
    if (*complete)
    { // if complete is true
      pthread_t rowthread, colthread, squarethred;
      for (int i = 1; i <= psize; i++)
      {
        parameters *Rowdata = (parameters *)malloc(sizeof(parameters));
        Rowdata->row = i; // check the row only
        Rowdata->complete = true;
        Rowdata->valid = true;
        Rowdata->grid = grid;
        Rowdata->psize = psize;
        pthread_create(&rowthread, NULL, RowCheck, Rowdata);

        pthread_join(rowthread, NULL);
        // printf("The value of valid is: %d\n", valid);
        // bool valid = Rowdata->valid;
        free(Rowdata);
      }
      for (int j = 1; j <= psize; j++)
      {
        parameters *Coldata = (parameters *)malloc(sizeof(parameters));
        Coldata->column = j;
        Coldata->complete = true;
        Coldata->valid = true;
        Coldata->grid = grid;
        Coldata->psize = psize;
        pthread_create(&colthread, NULL, ColumnCheck, Coldata);
        pthread_join(colthread, NULL);
        free(Coldata);
      }
      for (int i = 1; i <= psize; i++)
      {
        if (x == 1)
        {
          *valid = false;
        }
        else
        {
        }
      }
    }
  }

  // takes filename and pointer to grid[][]
  // returns size of Sudoku puzzle and fills grid
  int readSudokuPuzzle(char *filename, int ***grid)
  {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
      printf("Could not open file %s\n", filename);
      exit(EXIT_FAILURE);
    }
    int psize;
    fscanf(fp, "%d", &psize);
    int **agrid = (int **)malloc((psize + 1) * sizeof(int *));
    for (int row = 1; row <= psize; row++)
    {
      agrid[row] = (int *)malloc((psize + 1) * sizeof(int));
      for (int col = 1; col <= psize; col++)
      {
        fscanf(fp, "%d", &agrid[row][col]);
      }
    }
    fclose(fp);
    *grid = agrid;
    return psize;
  }

  // takes puzzle size and grid[][]
  // prints the puzzle
  void printSudokuPuzzle(int psize, int **grid)
  {
    printf("%d\n", psize);
    for (int row = 1; row <= psize; row++)
    {
      for (int col = 1; col <= psize; col++)
      {
        printf("%d ", grid[row][col]);
      }
      printf("\n");
    }
    printf("\n");
  }

  // takes puzzle size and grid[][]
  // frees the memory allocated
  void deleteSudokuPuzzle(int psize, int **grid)
  {
    for (int row = 1; row <= psize; row++)
    {
      free(grid[row]);
    }
    free(grid);
  }

  // expects file name of the puzzle as argument in command line
  int main(int argc, char **argv)
  {
    if (argc != 2)
    {
      printf("usage: ./sudoku puzzle.txt\n");
      return EXIT_FAILURE;
    }

    // grid is a 2D array
    int **grid = NULL;
    // find grid size and fill grid
    int sudokuSize = readSudokuPuzzle(argv[1], &grid);
    bool valid = false;
    bool complete = false;
    checkPuzzle(sudokuSize, grid, &complete, &valid);
    printf("Complete puzzle? ");
    printf(complete ? "true\n" : "false\n");
    if (complete)
    {
      printf("Valid puzzle? ");
      printf(valid ? "true\n" : "false\n");
    }
    printSudokuPuzzle(sudokuSize, grid);
    deleteSudokuPuzzle(sudokuSize, grid);
    return EXIT_SUCCESS;
  }
