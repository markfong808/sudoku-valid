// Sudoku puzzle verifier and solver
#include <string.h>
#include <math.h>
#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#define num_threads 27

// int result[num_threads] = {0};
int x = 0; // label the valid flag
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
  //bool valid = parm->valid;
  int psize = parm->psize;
  int **grid = parm->grid;
  int *valider = (int *)malloc((psize * psize) * sizeof(int));
  for (int i = 0; i <= (psize * psize); i++)
  {
    valider[i] = 0;
  }
  for (int j = 1; j <= psize; j++)
  {
    int num = grid[row][j];
    if (num < 1 || num > (psize * psize) || valider[num] == 1)
    {
      parm->valid = false;

      break;
    }
    else
    {
      valider[num] = 1;
    }
  }
  // printf("Valider array after the loop: ");
  // for (int i = 0; i <= psize * psize; i++)
  // {
  //   printf("%d ", valider[i]);
  // }
  // printf("\n");
  // printf("Valid after the loop: %s\n", valid ? "true" : "false");
  x = 2;
  free(valider);

  // printf("Thread result: %d\n" ,valid);
  // return (void *) valid;
}

void *ColumnCheck(void *par)
{

  parameters *parm = (parameters *)par;
  int col = parm->column;
  int psize = parm->psize;
  int **grid = parm->grid;
  //bool valid = parm->valid;

  int *valider = (int *)malloc((psize * psize) * sizeof(int));

  for (int i = 0; i <= (psize * psize); i++)
  {
    valider[i] = 0;
  }
  for (int i = 1; i <= psize; i++)
  {
    int num = grid[i][col];
    if (num < 1 || num > (psize * psize) || valider[num] == 1)
    {
      parm->valid = false;
      break;
    }
    else
    {
      valider[num] = 1;
    }
  }
  // printf("Valider array after the loop: ");
  // for (int i = 0; i <= psize * psize; i++)
  // {
  //   printf("%d ", valider[i]);
  // }
  // printf("\n");
  // printf("Valid after the loop: %s\n", valid ? "true" : "false");
  x = 2;

  free(valider);
}

void *SubgridCheck(void *par)
{
  parameters *parm = (parameters *)par;
  int row = parm->row;
  int col = parm->column;
  int psize = parm->psize;
  int **grid = parm->grid;
  //bool valid = parm->valid;

  int *valider = (int *)malloc((psize + 1) * sizeof(int));
  for (int i = 0; i <= psize; i++)
  {
    valider[i] = 0;
  }

  if (psize > 3)
  {
    int subgrid_size = (int)sqrt(psize);
    for (int row = 1; row <= psize; row += subgrid_size)
    {
      for (int col = 1; col <= psize; col += subgrid_size)
      {
        // Reset valider for each subgrid
        memset(valider, 0, (psize + 1) * sizeof(int));
        for (int bx = row; bx < row + subgrid_size; bx++)
        { // box_num inner loop
          for (int by = col; by < col + subgrid_size; by++)
          {
            int box_num = grid[bx][by];
            if (box_num < 1 || box_num > psize || valider[box_num] == 1)
            {
              parm->valid = false;
              // Handle breaking out of the innermost loop
              goto end_of_subgrid;
            }
            else
            {
              valider[box_num] = 1;
            }
          }
        }
      end_of_subgrid:;
      }
    }
  }
  x = 2;

  free(valider);
}

// Function to check if placing a number in a certain cell is valid
bool isValid(int psize, int **grid, int row, int col, int num)
{
  // Check if 'num' is not in the current row and column
  for (int i = 1; i <= psize; i++)
  {
    if (grid[row][i] == num || grid[i][col] == num)
    {
      return false;
    }
  }
  // Check if 'num' is not in the current 2x2 subgrid
  int subgridRowStart = (row - 1) / 2 * 2 + 1;
  int subgridColStart = (col - 1) / 2 * 2 + 1;

  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 2; j++)
    {
      if (grid[subgridRowStart + i][subgridColStart + j] == num)
      {
        return false;
      }
    }
  }

  return true;
}
// Function to find an empty cell in the puzzle
bool findEmptyCell(int psize, int **grid, int *row, int *col)
{
  for (*row = 1; *row <= psize; (*row)++)
  {
    for (*col = 1; *col <= psize; (*col)++)
    {
      if (grid[*row][*col] == 0)
      {
        return true; // Found an empty cell
      }
    }
  }
  return false; // No empty cell found
}

// Function to solve the Sudoku puzzle using backtracking
bool solveSudoku(int psize, int **grid)
{
  int row, col;

  // Find an empty cell
  if (!findEmptyCell(psize, grid, &row, &col))
  {
    // If no empty cell, the puzzle is solved
    return true;
  }

  // Try placing a number in the empty cell
  for (int num = 1; num <= psize; num++)
  {
    if (isValid(psize, grid, row, col, num))
    {
      // Place the number if it's valid
      grid[row][col] = num;

      // Recursively try to solve the rest of the puzzle
      if (solveSudoku(psize, grid))
      {
        return true; // Solution found
      }

      grid[row][col] = 0;
    }
  }

  return false; 
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
    if (!*complete)
    {
      // If the puzzle is incomplete, fill in the empty cells
      *valid = solveSudoku(psize, grid);
    }

    if (*complete)
    { // if complete is true
      pthread_t rowthread, colthread, subgridthreads;
      for (int i = 1; i <= psize; i++)
      {
        parameters *Rowdata = (parameters *)malloc(sizeof(parameters));
        Rowdata->row = i; // check the row only
        Rowdata->complete = true;
        Rowdata->valid = true;
        Rowdata->grid = grid;
        Rowdata->psize = psize;
        parameters *Coldata = (parameters *)malloc(sizeof(parameters));
        Coldata->column = i;
        Coldata->complete = true;
        Coldata->valid = true;
        Coldata->grid = grid;
        Coldata->psize = psize;
        pthread_create(&colthread, NULL, ColumnCheck, Coldata);
        pthread_create(&rowthread, NULL, RowCheck, Rowdata);

        pthread_join(rowthread, NULL);
        pthread_join(colthread, NULL);
        if (!Rowdata->valid || !Coldata->valid)
        {
          *valid = false;
        }
        // printf("The value of valid is: %d\n", valid);
        // bool valid = Rowdata->valid;
        free(Rowdata);
        free(Coldata);
      }

      for (int i = 1; i <= psize; i++) // 4x4 create 4 thread
      {
        parameters *SubGrid_data = (parameters *)malloc(sizeof(parameters));
        SubGrid_data->row = 1;
        SubGrid_data->column = 1;
        SubGrid_data->complete = true;
        SubGrid_data->valid = true;
        SubGrid_data->grid = grid;
        SubGrid_data->psize = psize;
        pthread_create(&subgridthreads, NULL, SubgridCheck, SubGrid_data);
        pthread_join(subgridthreads, NULL);
        if (!SubGrid_data->valid)
        {
          *valid = false;
        }
        free(SubGrid_data);
      }

      // if (x == 1)
      // {
      //   *valid = false;
      // }
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
