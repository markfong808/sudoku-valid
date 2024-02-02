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


# Check a Grid :

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
- The Sum AND Prod technic :

It was under your eyes since the beguining :

If you need to have ONLY one number each time by row / col / square, then, if you sum all the row, the result need to be ALWAYS the same, if not, then, maybe one number is missing, or is duplicate, etc ... Something is wrong. But you need to do Multiplications on it too, because you can have this grid true only with sum method :

```
1 1 | 4 4
4 4 | 1 1
---------
1 1 | 4 4
4 4 | 1 1
```
This is true with only Sum, so thats why we use Prod too.


So in our exemple, the Sum is :
1 + 2 + 3 + 4 = 10.

And the Prod is :
1 * 2 * 3 * 4 = 24.

For 9x9 grid (also call 3x3 sub grid / square) :
1 + 2 + 3 + 4 + 5 + 6 + 7 + 8 + 9 = 45.
And :
1 * 2 * 3 * 4 * 5 * 6 * 7 * 8 * 9 = 362880.

It is call `sum of natural numbers` and `factorial`.

How to find it simply ?
```
TOTAL_SUM = (N * (N + 1)) / 2					// Where N is the number of number in a row / col / square who are always the same !

TOTAL_PROD = 1;

for (Each N)	{
	TOTAL_PROD *= N - i;
}
```




So your code will look something like this :

```
int MySum = 0;
int MyProd = 1;
For (each number in my row / col)	{
	MySum += ActualNumber;
	MyProd *= ActualNumber;
}
If MySum != TOTAL_SUM
	Your row / col / square is not good !
If MyProd != TOTAL_PROD
	Your row / col / square is not good !
```

Note that the TOTAL_SUM and TOTAL_PROD need to be calc one time for the whole grid, so do it first, then put the result in an int, and put this int in you main struct.

Oh I forget, yes you need a global struct, with TOTAL_SUM in. Not in global, you just pass it as an arg at every function that request it !

------


Ok now you know how to check row and col easily, but whats about square ?

Well it is more difficult :

First do a sqrt to your mainGrid size to save the square size. put it in your main struct !

(Try to avoid using math lib when you can, that's why i create some sort of reverse sqrt in my code.
Why ? Because, if you call only one time this lib, its ok, but if you call it many time it will take lot of CPU time.)

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
And the same for TOTAL_PROD

It's easy to do just check all number in squareGrid and comapre it to TOTAL_SUM and TOTAL_PROD.


Perfect now you can check if a grid is valid or not !!!!


One more thing, here my main struct :

```
typedef struct	{
	int	psize;					// The size of the main grid, and the size of each row, col and squares
	int	sum;					// The SUM number
	int prod;					// The multiplication product
	int sSize;					// The size of a square
	int	**grid;					// The mainGrid
	int	**sGridSum;				// The squareGridSum, like for 4x4 you have 2x2 grid
	int	**sGridProd;			// The squareGridProd, like for 4x4 you have 2x2 grid
	int	incNbr;					// The number of incomplete points (See AutoComplete part)
	int	**iPos;					// The positions of incomplete points (See AutoComplete part)
	unsigned long long	maxTry;	// The Max try for Brute Force (See AutoComplete part)
} parameters;
```

------

# Threads Part :

The goal to this project is to implement Thread, but that realy easy, I recommand to do it AT THE END, when your are done with every things else. If you dont want to implement Auto Complete, then add Threads now. Instead go to AutoComplete part and then come back here !


Why implement the thread at the end ? It is C, you have to change code lot of time, even if you do realy clean code it could be difficult to just do little changes.

Thread are really easy : 

Actually i use it only into the check section, see `int checkValidThreads(parameters d, int print);` function.

So i create a thread for each type of check, row, col and square.
```
pthread_t thread1, thread2, thread3;

pthread_create(&thread1, NULL, checkRow, (void*)&d);
pthread_create(&thread2, NULL, checkCol, (void*)&d);
pthread_create(&thread3, NULL, checkSquareGrid, (void*)&d);
```

Then i wait they are done...

```
void* result1, *result2, *result3;		// If you whant to take some result...

pthread_join(thread1, &result1);
pthread_join(thread2, &result2);
pthread_join(thread3, &result3);
```

And i convert my results :

```
int resultValue1 = (int)(intptr_t)result1;
int resultValue2 = (int)(intptr_t)result2;
int resultValue3 = (int)(intptr_t)result3;
```

The results are in fact imple int's, which tell me whether it's good or not.

So, how `pthread_create()` work ?

Simple, it take 4 args :
* The pthread_t address,
* You dont need this actually, just put `NULL`,
* The name of the function you whant to excecute,
* One adresse... Who's the only arg you can give to your function ! The tips is to give the adresse to your struct, so your function have your struct...


If i have time i will implement few Threads for the Auto Complete part, after refound it !


------



# AutoComplete part :

Here's the big deal !

My algorithm use Brute Force to solve the missing numbers.

Brute Force take a lot of CPU time, so i have to implement an optimize version later.

For exemple if you have only one number to find, and your grid is like 4x4 size, your number is between 1 - 4.

So you try 4 number....

Ok now you have incomplete 2 numbers to find, and your grid is still 4x4, well you dont have just 4 + 4 = 8 possibilities, but you have 4 ^ 2 = 4 x 4 = 16 possibilities !

To make it simple :
* In my struct I have `int	incNbr;`, who's the number of incomplete numbers.
* To find how many maximum try you hate to do to find the right incNbr, calc this :  psize ^ incNbr, where psize is the mainGrid size.

But, ask you can imagine, if you have a mainGrid size 9x9, so 81, and 10 numbers to find, that not so much isnt it ? ok, 9 ^ 10 is like : 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 = 3 486 784 401 trials !!!!

Do not forget, for each trial, you change only ONE number, and then you have to Check for :
* Row, is ok ? then continue : else go change ONE number;
* Col, is ok ? then continue : else go change ONE number;
* Square, is ok ? then you finish the Sudoku ! : else go change ONE number;

So yes, that's why we use the SUM method to check instead of checking every number, it's take less time to CPU...

How to Brute Force easily on this project ?

First I was thinking about Double Linked List (I love it), but I found an easy way for the CPU :
A grid (One more), in my struct it is `int	**iPos;`.

Before looking at it, let's do a little detour :

Remember the incomplete number are all `0`.

As you can guess, my grid iPos will contain info about the missing numbers.

But The processor need to save time.

So i change every `0` by negatif numbers, starting at `-1`.

Why neg numbers ? Well you can easily know if it's a regular number or not by checking the sign ^^, and just use this function to perform tests on it :

```
// return the absolute value
int	abs(int i)	{
	return (i > 0) ? i : -i ;
}
```

So just increase (ok here we decrease the number but i prefer to say increase...) your number each time it's needed.

Lets go back to our grid :

```
iPos[incomplete_numbers_in_Grid][2];
```

For exemple the first element can be :

```
iPos[0][1] = Coordinate X of the incomplete number 0
iPos[0][2] = Coordinate Y of the incomplete number 0

iPos[1][1] = Coordinate X of the incomplete number 1
iPos[1][2] = Coordinate Y of the incomplete number 1

...
```

I think you've got it.


I hope I didn't make too many spelling mistakes...
