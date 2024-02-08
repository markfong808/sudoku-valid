#!/bin/bash

# Script to compile and run sudoku program
# make re
# gcc -Wall -Wextra sudoku.c -o sudoku
# gcc -pthread -Wall -Wextra -Werror sudoku-kscamy.c -o sudoku
# ./sudoku puzzles/puzzle2-invalid.txt
# ./sudoku puzzles/puzzle2-valid.txt
# ./sudoku puzzles/puzzle2-fill-valid.txt
# ./sudoku puzzles/puzzle2-fill-valid-2.txt
# ./sudoku puzzles/puzzle2-fill-valid-3.txt
# ./sudoku puzzles/puzzle2-fill-valid-2-hard.txt
# ./sudoku puzzles/puzzle9-valid.txt
./sudoku puzzles/puzzle9-fill-valid.txt
# ./sudoku puzzles/puzzle9-fill-valid-hard.txt
# ./sudoku puzzles/puzzle16-valid.txt
# ./sudoku puzzles/puzzle16-invalid.txt
# ./sudoku puzzles/puzzle16-fill-valid.txt



# to check for memory leaks, use
# valgrind ./sudoku puzzles/puzzle9-good.txt

# to fix formating use
# clang-format -i main.c

# if clang-format does not work 
# use 'source scl_source enable llvm-toolset-7.0' and try again

# if using GitHub, you can run the program on GitHub servers and see
# the result. Repository > Actions > Run Workflow


