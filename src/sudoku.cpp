#include "sudoku.h"
#include <algorithm>

using namespace std;

Sudoku::Sudoku() : numEmptyCells(9*9) {
    // Set state to contain all zeros
    for (auto it = state.begin(); it != state.end(); ++it) {
        for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
            *it2 = 0;
        }
    }

    // Set possible values to contain all values from 1 to 9
    for (auto it = values.begin(); it != values.end(); ++it) {
        for (auto it2 = it->begin(); it2 != it->end(); ++it2) {
            for (int i=1; i<=9; ++i) {
                it2->push_back(i);
            }
        }
    }
}

bool Sudoku::isConsistent(int x, int y, int value) const {
    if (value == 0) return true;

    // If cells along the same row and column contain value, failure
    for (int i=0; i<9; ++i) {
        if (i != x && state[i][y] == value) return false;
        if (i != y && state[x][i] == value) return false;
    }

    // Check 3x3 subgrid
    for (int i = x/3*3; i < x/3*3 + 3; ++i) {
        for (int j = y/3*3; j < y/3*3 + 3; ++j) {
            if (state[i][j] == value && (i != x || j != y)) return false;
        }
    }

    // All constraints passed
    return true;
}

bool Sudoku::isSolved() const {
    // Do an easy check first before complete check
    if (getNumEmptyCells() != 0) return false;

    // Maintain visited digits for the rows, columns, and 3x3 subgrids.
    // That is, rowNums[i][n] states whether row i contains the digit n.
    bool rowNums[9][9]{}; // Default initialize to false
    bool colNums[9][9]{};
    bool subgridNums[3][3][9]{};

    // Single pass through every cell, flagging the seen digits. If a digit
    // was already seen, then board state is invalid. Otherwise, board state
    // is a valid solution.
    int n;
    for(int row=0; row<9; ++row){
        for(int col=0; col<9; ++col){

            n = state[row][col]-1;
            if (n <= -1) continue;

            if (rowNums[row][n] || colNums[col][n]
                || subgridNums[row/3][col/3][n])
                return false;

            rowNums[row][n] = true;
            colNums[col][n] = true;
            subgridNums[row/3][col/3][n] = true;
        }
    }

    return true;
}

bool Sudoku::isPossibleValue(int x, int y, int value) const {
    assertCell(x, y, value);

    if (find(values[x][y].begin(), values[x][y].end(), value) != values[x][y].end()) {
        return true;
    }
    return false;
}

void Sudoku::initCell(int x, int y, int value) {
    assertCell(x, y, value);

    setCell(x, y, value);

    // Remove inconsistent possible values
    int minX = x/3*3;
    int minY = y/3*3;

    // Remove inconsistent values from same 3x3 subgrid
    for (int i = minX; i < minX+3; ++i) {
        for (int j = minY; j < minY+3; ++j) {
            values[i][j].remove(value);
        }
    }

    // Remove inconsistent values from same row, excluding the already
    // counted 3x3 subgrid.
    for (int i=0; i<minX; ++i) {
        values[i][y].remove(value);
    }
    for (int i=minX+3; i<9; ++i) {
        values[i][y].remove(value);
    }

    // From same column
    for (int j=0; j<minY; ++j) {
        values[x][j].remove(value);
    }
    for (int j=minY+3; j<9; ++j) {
        values[x][j].remove(value);
    }
}

void Sudoku::setCell(int x, int y, int value) {
    assertCell(x, y, value);

    if (value != 0 && state[x][y] == 0) {
        --numEmptyCells;
    } else if (value == 0 && state[x][y] != 0) {
        ++numEmptyCells;
    }

    state[x][y] = value;
}

bool Sudoku::addValue(int x, int y, int value) {
    assertCell(x, y, value);

    // If values[x][y] doesn't contain value
    if (find(values[x][y].begin(), values[x][y].end(), value)
        == values[x][y].end()) {

        // Add it
        values[x][y].push_back(value);
        return true;
    }
    return false;
}

bool Sudoku::removeValue(int x, int y, int value) {
    assertCell(x, y, value);

    auto iter = find(values[x][y].begin(), values[x][y].end(), value);
    if (iter != values[x][y].end()) {
        values[x][y].erase(iter);
        return true;
    }
    return false;
}
