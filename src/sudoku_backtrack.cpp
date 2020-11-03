#include "sudoku_backtrack.h"
using namespace std;

array<int, 2> SudokuBacktrack::getNextVar(const Sudoku& board) const {
    assert(board.getNumEmptyCells() > 0);

    // If using minimum remaining values heuristic
    if (heuristic == 3) {
        int minValues = 9;
        vector<array<int, 2>> leastVars;

        // Find empty cells with least remaining possible values and return it
        for (int i=0; i<9; ++i) {
            for (int j=0; j<9; ++j) {
                if (board.isEmpty(i, j)) {
                    int size = board.getValues(i, j).size();

                    if (size < minValues) {
                        leastVars.clear();
                        minValues = size;
                    }
                    if (size == minValues) {
                        leastVars.push_back(array<int, 2>{i, j});
                    }
                }
            }
        }
        if (leastVars.size() == 1) return leastVars.front();

        int maxConstraints = -1;
        array<int, 2> bestVar{-1, -1};

        // If multiple cells, use most constraining variable as tiebreaker.
        // For each cell in leastVars
        for (const array<int, 2> &var : leastVars) {

            int constraints = 0;

            int x = var[0];
            int y = var[1];
            int minX = x/3*3;
            int minY = y/3*3;

            // Count number of empty cells in 3x3 subgrid
            for (int i = minX; i < minX+3; ++i) {
                for (int j = minY; j < minY+3; ++j) {
                    if (i != x && j != y && board.isEmpty(i, j)) {
                        ++constraints;
                    }
                }
            }

            // Count number of empty cells in row, excluding the 3x3 subgrid
            // already counted
            for (int i=0; i<minX; ++i) {
                if (board.isEmpty(i, y)) ++constraints;
            }
            for (int i=minX+3; i<9; ++i) {
                if (board.isEmpty(i, y)) ++constraints;
            }

            // Count number of empty cells in column
            for (int j=0; j<minY; ++j) {
                if (board.isEmpty(x, j)) ++constraints;
            }
            for (int j=minY+3; j<9; ++j) {
                if (board.isEmpty(x, j)) ++constraints;
            }

            // Set bestVar to cell with most number of constraints
            if (constraints > maxConstraints) {
                maxConstraints = constraints;
                bestVar = var;
            }
        } // End for each cell in leastVars

        assert(bestVar[0] != -1);
        return bestVar;
    }
    // Else, not using heuristic for getting next variable
    else {
        // Return any empty cell
        for (int i=0; i<9; ++i) {
            for (int j=0; j<9; ++j) {
                if (board.isEmpty(i, j)) {
                    return array<int, 2>{i, j};
                }
            }
        }
    }

    // No empty cell found. Should never reach this point.
    assert(false);
    return array<int, 2>{0, 0};
}

list<int> SudokuBacktrack::getValues(const Sudoku& board, int x, int y) const {

    // If using least constraining value heuristic
    if (heuristic == 3) {
        auto values = board.getValues(x, y);

        // constraintPairs is a vector of pairs of numbers. First number of the pair is a
        // value of cell (x, y), second number is the number of constraints for that value.
        // ex. If the board is full, then the constraintPairs[i][1] = 0 for all i.
        //     If cell (x, y) only has a possible value of 9, then constraintPairs[0][0] = 9.
        //     In addition, if all other cells in the same row, column, and 3x3 subgrid are
        //     filled except for one cell that also has a possible value of 9, then
        //     constraintPairs[0][1] = 1.
        vector<array<int, 2>> constraintPairs;
        constraintPairs.reserve(values.size());

        // For each possible value of cell (x, y)
        for (int val : board.getValues(x, y)) {

            int constraints = 0;
            int minX = x/3*3;
            int minY = y/3*3;

            // Count number of empty cells in 3x3 subgrid that has the same
            // possible value.
            for (int i = minX; i < minX+3; ++i) {
                for (int j = minY; j < minY+3; ++j) {
                    if (i != x && j != y && board.isEmpty(i, j)
                        && board.isPossibleValue(i, j, val)) {
                        ++constraints;
                    }
                }
            }

            // Count number of empty cells in row that has the same possible
            // value, excluding the 3x3 subgrid already counted
            for (int i=0; i<minX; ++i) {
                if (board.isEmpty(i, y) && board.isPossibleValue(i, y, val))
                    ++constraints;
            }
            for (int i=minX+3; i<9; ++i) {
                if (board.isEmpty(i, y) && board.isPossibleValue(i, y, val))
                    ++constraints;
            }

            // Count in column
            for (int j=0; j<minY; ++j) {
                if (board.isEmpty(x, j) && board.isPossibleValue(x, j, val))
                    ++constraints;
            }
            for (int j=minY+3; j<9; ++j) {
                if (board.isEmpty(x, j) && board.isPossibleValue(x, j, val))
                    ++constraints;
            }

            constraintPairs.push_back(array<int, 2>{val, constraints});
        } // End for each possible value of cell (x, y)

        // Order the possible values from least constraining to most constraining
        sort(constraintPairs.begin(), constraintPairs.end(),
             [](const auto& a, const auto& b) { return a[1] < b[1]; });
        transform(constraintPairs.begin(), constraintPairs.end(), values.begin(),
                  [](auto& p) { return move(p[0]); });

        return values;
    }
    // Else, not using heuristic. Return the possible values unaltered.
    else {
        return board.getValues(x, y);
    }
}

bool SudokuBacktrack::forwardCheck(Sudoku& board, int x, int y
                      , vector<array<int, 2>> &removedVars) const {

    if (heuristic == 2 || heuristic == 3) {
        removedVars.clear();
        int value = board.getCell(x, y);

        int minX = x/3*3;
        int minY = y/3*3;

        // For each cell in the same 3x3 subgrid
        for (int i = minX; i < minX + 3; ++i) {
            for (int j = minY; j < minY + 3; ++j) {
                if (i == x && j == y) continue;

                // Remove value
                if (board.removeValue(i, j, value)) {
                    removedVars.push_back(array<int, 2>{i, j});
                }
                // // If empty cells have no more possible values, then failure
                if (board.isEmpty(i, j) && board.getValues(i, j).size() == 0) {
                    return false;
                }
            }
        }

        // Check cells along same row, excluding the already checked
        // 3x3 subgrid
        for (int i=0; i<minX; ++i) {
            if (board.removeValue(i, y, value)) {
                removedVars.push_back(array<int, 2>{i, y});
            }
            if (board.isEmpty(i, y) && board.getValues(i, y).size() == 0) {
                return false;
            }
        }
        for (int i=minX+3; i<9; ++i) {
            if (board.removeValue(i, y, value)) {
                removedVars.push_back(array<int, 2>{i, y});
            }
            if (board.isEmpty(i, y) && board.getValues(i, y).size() == 0) {
                return false;
            }
        }

        // Check cells in same column
        for (int j=0; j<minY; ++j) {
            if (board.removeValue(x, j, value)) {
                removedVars.push_back(array<int, 2>{x, j});
            }
            if (board.isEmpty(x, j) && board.getValues(x, j).size() == 0) {
                return false;
            }
        }
        for (int j=minY+3; j<9; ++j) {
            if (board.removeValue(x, j, value)) {
                removedVars.push_back(array<int, 2>{x, j});
            }
            if (board.isEmpty(x, j) && board.getValues(x, j).size() == 0) {
                return false;
            }
        }
    }
    return true;
}

void SudokuBacktrack::revertInferences(Sudoku& board, int value
                      , vector<array<int, 2>> &removedVars) const {

    for (auto it = removedVars.begin(); it != removedVars.end(); ++it) {
        bool success = board.addValue((*it)[0], (*it)[1], value);
        assert(success);
    }
    removedVars.clear();
}

bool SudokuBacktrack::solve(Sudoku& board) const {
    // Check if board is solved
    if (board.isSolved()) return true;
    if (board.getNumEmptyCells() == 0) return false;

    // Get the next variable and its possible values
    array<int, 2> loc = getNextVar(board);
    int x = loc[0];
    int y = loc[1];
    list<int> values = getValues(board, x, y);

    // For each possible value of cell (x, y)
    for (int value : values) { // (auto it = values.begin(); it != values.end(); ++it) {

        vector<array<int, 2>> removedVars;
        int oldValue = board.getCell(x, y);
        // int value = *it;

        // If value is consistent
        if (board.isConsistent(x, y, value)) {
            // Set cell to it
            board.setCell(x, y, value);

            // If forward check is consistent
            if (forwardCheck(board, x, y, removedVars)) {
                // Solve for current board state
                if (solve(board)) return true;
            }
        }

        board.setCell(x, y, oldValue);
        revertInferences(board, value, removedVars);
    }

    return false;
}
