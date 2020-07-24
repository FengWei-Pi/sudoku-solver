# sudoku-solver

Soduku Solver is a small, interactive C++ console program that implements backtracking search to solve sudoku puzzles as a constraint satisfaction problem. The program reads in a file containing a sudoku puzzle from the command line and outputs a solution, if it exists. Example files containing sudoku puzzles are included.

The binary executable can be downloaded [here](https://github.com/FengWei-Pi/sudoku-solver/releases/).

Commands include:
- **solve file** Attempts to solve the sudoku puzzle in the file.
- **set heuristic x** Sets the heurstic for backtracking search according to x, where x can be 1, 2 or 3. If x is 1, then no heuristic is used. If x is 2, then forward checking is used. If x is 3, then forward checking plus minimum remaining values, most contraining variable, and least constraining value is used.

### Sudoku
Sudoku is single player puzzle game played on a 9x9 grid. Each grid cell can contain the digits 1 to 9. Given some initially filled squares, the objective is to fill the remaining, empty squares so that every row, column, and 3x3 subgrid contain no duplicates. That is, every row, column, and 3x3 subgrid contain each digit from 1 to 9 exactly once.

More info can be found [here](https://en.wikipedia.org/wiki/Sudoku). Sudoku can be played [here](https://www.websudoku.com/).

## Constraint Satisfaction Problems
Constraint Satisfaction Problems (CSPs) are problems that can be expressed as a set of variables, a domain of values for each variable, and a set of contraints or rules that determine valid value assignments to variables. The goal is to assign values to all the variables such that all the constraints are satisfied.

For a sudoku, this means that each grid cell is a variable, the domain for every cell is {1, 2, 3, ..., 9}, and the constraints are that cell values in the same row, column, and 3x3 subgrid are distinct.

CSPs can be solved with backtracking search. Backtracking search works by trying to assign values to variable, reverting assignments if they don't work, and trying a different assignment. First, a variable is selected. Next, values are selected from the variable's domain. If a value is found that doesn't violate a constraint, then the value is assigned, and the algorithm selectes another variable to assign a value. If all values for the current variable violate a constraint, then the algorithm reverts the current value assigment and selects values for a previously assigned variable. It continues this cycle of going as deep as it can, assigning values to variables, and backtracking when value assignments fail until a solution is found or all possibilities are exhausted and no solution exists.

For more info, see the section on constraint satisfaction problems in the book _Artificial Intelligence: A Modern Approach_ by Peter Norvig and Stuart J. Russell.

### Heuristics
Heuristics can be used when assigning a value and selecting a variable or value to speed up the search.

Forward checking removes immediately inconsistent values from the domain of other variables when a variable is assigned a value. For sudoku, this means when a grid cell is assigned a value, all other empty grid cells in the same row, column, and 3x3 subgrid have the value removed from their domains. This an intuitive but powerful heuristic.

Minimum remaining values (MRV) and most contraining variable (MCV) are heuristics that build on this. These heuristics select variables to assign in a specific order, and can have different names. MRV selects the next variable to assign that has the least number of values in its domain left. If the current assignment of variables will never result in a solution, then MRV would let us know first. If there are multiple variables with the lowest domain size, then MCV selects the variable that is involved in the most constraints. For sudoku, this means a grid cell is selected that has the most number of empty cells in the same row, column, and 3x3 subgrid. MCV is used as a tiebreaker for MRV. Random selection is used as a tiebreaker after that.

Least constraining value (LCV) is used after a variable is selected. LCV selects the value for the variable that is involved in the least number of constraints. Since we've already selected a variable to assign, LCV rules out the fewest number of value assignments for other variables. If there is a solution for the current variable assignments, LCV can find it faster.
