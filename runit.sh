#!/bin/bash

# Script to compile and run sudoku program
rm -f sudoku
# gcc -Wall -Wextra sudoku.c -o sudoku
gcc -pthread -Wall -Wextra -Werror sudoku-kscamy.c -o sudoku
# ./sudoku puzzle2-invalid.txt
# ./sudoku puzzle2-valid.txt
# ./sudoku puzzle2-fill-valid.txt
# ./sudoku puzzle2-fill-valid-2.txt
# ./sudoku puzzle2-fill-valid-3.txt
# ./sudoku puzzle2-fill-valid-2-hard.txt
# ./sudoku puzzle9-valid.txt
# ./sudoku puzzle9-fill-valid-hard.txt
# ./sudoku puzzle16-valid.txt
# ./sudoku puzzle16-invalid.txt
./sudoku puzzle16-fill-valid.txt



# to check for memory leaks, use
# valgrind ./sudoku puzzle9-good.txt

# to fix formating use
# clang-format -i main.c

# if clang-format does not work 
# use 'source scl_source enable llvm-toolset-7.0' and try again

# if using GitHub, you can run the program on GitHub servers and see
# the result. Repository > Actions > Run Workflow


