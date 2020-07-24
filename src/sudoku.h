#pragma once

#include <cassert>
#include <algorithm>
#include <list>
#include <array>

// Sudoku is a puzzle game. The objective is to fill out a 9 by 9 grid
// with numbers ranging from 1 to 9 so that no row, column, or smaller
// 3x3 grid has a repeating digit. A sudoku puzzle initially has some
// cells filled. The player then fills the rest. For more info, see
// https://en.wikipedia.org/wiki/Sudoku

// Sudoku class implements a sudoku board, along with some helpful members
// and functions useful for backtracking, like possible values each cell
// can take on and functions to add or remove possible vaues.
// Sudoku uses coordinates (0, 0) for the top left cell, (8, 0) for the top
// right cell.
class Sudoku {
    // State represents a (partially) filled 9x9 sudoku board. state[0][0]
    // is the top left cell, and state[8][0] is the top right cell. A cell
    // with a value of zero means it's empty/unfilled. Otherwise, cells have
    // integer values ranging from 1 to 9.
    std::array<std::array<int, 9>, 9> state;

    // A 9x9 array where each element is a list. Each list stores the values
    // that its cell can take, according to the current inferences. Values
    // range from 1 to 9. Manipulated by SudukuBacktrack class.
    std::array<std::array<std::list<int>, 9>, 9> values;

    int numEmptyCells;

    // Asserts that (x, y) is a valid cell location.
    inline void assertCell(int x, int y) const {
        assert(0 <= x && x <= 8 && 0 <= y && y <= 8);
    }
public:
    Sudoku();

    // Returns true if placing value at cell (x, y) doesn't violate any
    // sudoku rules in the current board state.
    // requires: 0 <= x <= 8
    //           0 <= y <= 8
    //           0 <= value <= 9
    bool isConsistent(int x, int y, int value) const;

    // Returns true if board state is a valid sudoku solution.
    bool isSolved() const;

    // Returns true if values[x][y] contains value. False otherwise.
    // requires: 0 <= x <= 8
    //           0 <= y <= 8
    //           0 <= value <= 9
    bool isPossibleValue(int x, int y, int value) const;

    // Returns true if cell (x, y0 is unfilled. False otherwise.
    inline bool isEmpty(int x, int y) const {
        return (getCell(x, y) == 0);
    }

    // Getters
    // Get number of empty cells in current board state
    inline int getNumEmptyCells() const {
        return numEmptyCells;
    }

    // Get value at cell (x, y)
    // requires: 0 <= x <= 8
    //           0 <= y <= 8
    inline int getCell(int x, int y) const {
        assertCell(x, y);
        return state[x][y];
    };

    // Returns a constant reference to the board state.
    inline const std::array<std::array<int, 9>, 9>& getState() const {
        return state;
    };

    // Returns a constant reference to the possible values of cell (x, y).
    // requires: 0 <= x <= 8
    //           0 <= y <= 8
    inline const std::list<int>& getValues(int x, int y) const {
        return values[x][y];
    }

    // Sets the value of cell (x, y). Also removes inconsistent possible
    // values from other cells.
    // requires: 0 <= x <= 8
    //           0 <= y <= 8
    //           0 <= value <= 9
    void initCell(int x, int y, int value);

    // Sets the value of cell (x, y).
    // requires: 0 <= x <= 8
    //           0 <= y <= 8
    //           0 <= value <= 9
    void setCell(int x, int y, int value);

    // Adds value to values at cell (x, y) if it wasn't there before.
    // Returns true if value was added, false otherwise.
    // requires: 0 <= x <= 8
    //           0 <= y <= 8
    //           0 <= value <= 9
    bool addValue(int x, int y, int value);

    // Removes value from values at cell (x, y) if it's currently there.
    // Returns true if value was removed, false otherwise.
    // requires: 0 <= x <= 8
    //           0 <= y <= 8
    //           0 <= value <= 9
    bool removeValue(int x, int y, int value);
};
