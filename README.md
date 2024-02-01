# Sudoku verifier and solver

Works on sudoku puzzles of any size.
Uses multiple threads to check if a puzzle is valid.

For puzzles that have any "0"s, tries to find a valid number for the 0. Can solve simple puzzles where no backtracking is required.

4x4 puzzle, have 4 sub grid call *square*, each square is 2x2 size here.

To explain :

**[1][1]** = **3**, **[1][4]** = **1**, **[4][1]** = **4**, **[4][4]** = **0**

Yes we dont start at **0** even if we are in **C**.
```
3 0 | 0 1
2 1 | 0 0
---------
0 0 | 0 2
4 2 | 1 0
```

We can have more bigger puzzle but it respect one rule :

If you whant the right next sudoku size, you take the square size, and add 1, so you get here 3x3.

but instead of having 4 square in a puzzle with grid size 6x6, you have 9 square, each one 3x3, in total your grid is 9x9.

Same if you whant the right next one, take 4x4 squares size, sou you have 4x4=16 square in the grid, grid size is 16x16.

Etc, for 5 you have square size of 5x5, 5x5=25 squares in grid, grid size = 25x25...

Lets go back to the first grid, but complete this time :

```
3 4 | 2 1
2 1 | 3 4
---------
1 3 | 4 2
4 2 | 1 3

```


To complete a sudoku, you need 3 things :

* Each Row need each number one time,
* Each Colimn need each number one time,
* Each Square need each number one time.

What does it means ?
* First, if your puzzle grids size is 4x4, your numbers are 1, 2, 3, 4. No more, no less.
* Second, you have two way to check row / col / square :
------
- One by One, something like :
```
for (each number in my row / col)	{
	Check if the number is in my range 1 - 4, then		// not en obligation, but you know I'm a C guys...
	Check if the number is already in my CheckList, then
		Break or,
	Add this number to my CheckList, then				// Why this list ? Well if you have two time number 3 for exemple...
	go to next number.
}
When Done with this row, Clean the List !!!!			// You wont crash because your number is obviously already in this list...
Go to: Next Row.
```
------
- The Sum technic :

It was under your eyes since the beguining :

If you need to have ONLY one number each time by row / col / square, then, if you sum all the row, the result need to be ALWAYS the same, if not, then, maybe one number is missing, or is duplicate, etc ... Something is wrong.

So in our exemple, the sum is :
1 + 2 + 3 + 4 = 10.

For 9x9 grid (also call 3x3 sub grid / square) :
1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 = 45.

It is call `sum of natural numbers`

How to find it simply ?
```
TOTAL_SUM = (N * (N + 1)) / 2
```

So your code will look something like this :

```
TOTAL_SUM = (N * (N + 1)) / 2				// Where N is the number of number in a row / col / square who are always the same !
int MySum = 0;
For (each number in my row / col)	{
	MySym += ActualNumber;
}
If MySum != TOTAL_SUM
	Your row / col / square is not good !
```

Note that the TOTAL_SUM need to be calc one time for the whole grid, so do it first, then put the result in an int, and put this int in you main struct.

Oh I forget, yes you need a global struct, with TOTAL_SUM in.

Ok now you know how to check row and col easily, but whats about square ?

Well it is more difficult :

First do a sqrt to your mainGrid size to save the square size. put it in your main struct !

One simple way is to do SUM too, but check square one by one is'nt easy so, why not check all square ?

To do it, you need une new grid like sGrid (SquareGrid), size of numberOfGrid x numberOfGrid.

For exemple, with mainGrid like 4x4, 2x2 square, your sGrid is 2x2.

Init it with `0`.

Then its simple, just have to check by row like :


```
for (each row)	{				// for mainGrid
		for (each col)	{		// for mainGrid
			squareGrid[(((col - 1) / squareSize) + 1)] [(((row - 1) / squareSize) + 1 )] += mainGrid[col][row];
			// So it will SUM the mainGrid value and put it squareGrid, row by row.
		}
	}
```

So now you have a grid with all the square SUM of mainGrid.

You need to check if all SUMM are equal to ... TOTAL_SUM !

It's easy to do just check all number in squareGrid and comapre it to TOTAL_SUM.


Perfect now you can check if a grid is valid or not !!!!

I hope I didn't make too many spelling mistakes...

One more thing, here my main struct :

```
typedef struct	{
	int	psize;						// The size of the main grid, if your grid is 9x9, then psize is 9.
	int	sum;						// the SUM number
	int sSize;						// The size of a square
	int	**grid;						// The mainGrid !
	int	**sGrid;					// The squareGrid, like for 4x4 mainGrid you have 2x2 squareGrid
	int	incNbr;						// The number of incomplete points (See AutoComplete part)
	int	**iPos;						// positions of incomplete points (See AutoComplete part)
	unsigned long long	maxTry;		// The Max try for Brute Force, yes its too much (See AutoComplete part)
} parameters;
```

See AutoComplete part : Coming Soon !