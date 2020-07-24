#include "sudoku.h"
#include <exception>

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
            if (i == x && j == y) continue;
            if (state[i][j] == value) return false;
        }
    }

    // All constraints passed
    return true;
}

bool Sudoku::isSolved() const {
    // Do an easy check first before complete check
    if (getNumEmptyCells() != 0) return false;

    // TODO: Kinda inefficient. I think it's the same big O class, but
    // around twice as expensive over the next brute force approach.
    for (int i=0; i<9; ++i) {
        for (int j=0; j<9; ++j) {
            if (!isConsistent(i, j, getCell(i, j)))
                return false;
        }
    }

    return true;
}

bool Sudoku::isPossibleValue(int x, int y, int value) const {
    assertCell(x, y);
    assert(0 <= value && value <= 9);

    if (find(values[x][y].begin(), values[x][y].end(), value) != values[x][y].end()) {
        return true;
    }
    return false;
}

void Sudoku::initCell(int x, int y, int value) {
    if (value == 0) return;
    if (x < 0 || x > 8 || y < 0 || y > 8 || value < 1 || value > 9) {
        throw runtime_error("initCell arguments out of range");
    }

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
    assertCell(x, y);
    assert(0 <= value && value <= 9);

    if (value != 0 && state[x][y] == 0) {
        --numEmptyCells;
    } else if (value == 0 && state[x][y] != 0) {
        ++numEmptyCells;
    }

    state[x][y] = value;
}

bool Sudoku::addValue(int x, int y, int value) {
    assertCell(x, y);
    assert(1 <= value && value <= 9);

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
    assertCell(x, y);
    assert(1 <= value && value <= 9);

    auto iter = find(values[x][y].begin(), values[x][y].end(), value);
    if (iter != values[x][y].end()) {
        values[x][y].erase(iter);
        return true;
    }
    return false;
}
