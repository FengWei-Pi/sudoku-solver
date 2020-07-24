#pragma once

#include <vector>
#include "sudoku.h"

// SudokuBacktrack implements the backtracking algorithm for a sudoku puzzle.
// Optional heuristics can be set to improve the search.
class SudokuBacktrack {
    // Flag for which heuristic to use.
    // 1 = no heuristic
    // 2 = forward checking
    // 3 = forward checking with minimum remaining values, most constraining
    //     variable, and least constraining value
    // All other values = no heuristic
    int heuristic;

    // Returns the location of the next empty cell of board.
    // ex. On an empty board, getNextVar returns (0, 0).
    // requires: board has >= 1 empty cell
    std::array<int, 2> getNextVar(const Sudoku& board) const;

    // Returns the possible values of the cell at (x, y) of board.
    std::list<int> getValues(const Sudoku& board, int x, int y) const;

    // Performs forward checking by removing the value at cell (x, y) from
    // the possible values of cells in the same row, column, and 3x3 subgrid.
    // Returns true if possible values are all non-empty, false otherwise.
    // Stores the cell locations of the removed value in removedVars.
    // effects: board may change
    //          removedVars may change
    bool forwardCheck(Sudoku& board, int x, int y
                      , std::vector<std::array<int, 2>> &removedVars) const;

    // Reverts the values removed by forwardCheck. Given the cell locations of
    // the removed value, removedVars, adds the value to the possible values of
    // those cells.
    // effects: board may change
    //          removedVars will be empty
    void revertInferences(Sudoku& board, int value
                          , std::vector<std::array<int, 2>> &removedVars) const;

public:
    inline void setHeuristic(int h) {heuristic = h;}

    // Given a initial partially filled sudoku board, returns true if a
    // solution exists, false otherwise. If a solution exists, then the board
    // state will conatin the solution. It will contain garbage values otherwise.
    // effects: board may change
    bool solve(Sudoku& board) const;
};
