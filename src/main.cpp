#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <exception>
#include <chrono>
#include "sudoku_backtrack.h"

using namespace std;
// C:\Users\fengw\Desktop\sudoku.txt

// Prints the sudoku
void print(const Sudoku &s) {
    for (int y=0; y<9; ++y) {
        for (int x=0; x<9; ++x) {
            cout << s.getCell(x, y) << " ";
        }
        cout << endl;
    }
}

// Returns the sudoku created from the file
Sudoku read(ifstream &file) {

    // Keeps track of which cell to write to
    int x=0;
    int y=0;

    Sudoku s;
    string line;

    // Read lines from file
    while (getline(file, line)) {
        if (y >= 9) {
            throw runtime_error("File has too many lines");
        }

        istringstream iss(line);

        // Read characters from line
        while (iss >> line) {
            if (x >= 9) {
                throw runtime_error("File has too many characters in line");
            }
            if (line.size() > 1 || !isdigit(line[0])) {
                throw runtime_error("File has character " + line + " that is not a digit");
            }

            s.initCell(x, y, line[0] - '0');
            ++x;
        }
        x=0;
        ++y;
    }

    if (y < 9) {
        throw runtime_error("File has too few lines");
    }

    return s;
}

int main()
{
    cout << "Name: Sudoku Solver" << endl;
    cout << "Author: FengWei Pi" << endl << endl;
    cout << "Welcome to Sudoku Solver!" << endl;
    cout << "Commands:" << endl;
    cout << "> solve filename" << endl;
    cout << "> set heuristic 1/2/3" << endl;

    SudokuBacktrack solver;
    string cmd;

    // Keep reading commands from cin
    while (true) {
        cout << endl << "Please enter a command:" << endl;
        if (!getline(cin, cmd)) break;

        istringstream iss(cmd);
        getline(iss, cmd, ' ');

        // Handle solve command
        if (cmd == "solve") {
            getline(iss, cmd);

            // Open file
            ifstream file(cmd);
            if (!file.is_open()) {
                cout << "File " << cmd << " not found." << endl;
                continue;
            }

            // Solve
            try {
                Sudoku sudoku = read(file);

                cout << "Read in sudoku" << endl;
                print(sudoku);

                auto start = chrono::high_resolution_clock::now();
                auto finish = start;

                if (solver.solve(sudoku)) {
                    finish = std::chrono::high_resolution_clock::now();

                    cout << endl << "A solution is" << endl;
                    print(sudoku);
                } else {
                    finish = std::chrono::high_resolution_clock::now();

                    cout << "Sudoku has no solution" << endl;
                }
                auto timeTaken = chrono::duration_cast<chrono::milliseconds>(finish - start).count();
                cout << "Took " << timeTaken << " milliseconds" << endl;

            } catch (exception &e) {
                cout << e.what() << endl;
            }
        }
        // Handle set heuristic
        else if (cmd == "set" && iss >> cmd && cmd == "heuristic") {
            if (!(iss >> cmd)) {
                cout << "No digit entered" << endl;
                continue;
            }

            int val = cmd[0] - '0';
            if (val < 1 || val > 3) {
                cout << "Character " << cmd << " is not 1, 2, or 3" << endl;
                continue;
            }

            solver.setHeuristic(val);
            cout << "Heuristic " << val << " set. ";
            if (val == 1) {
                cout << "No heuristic used." << endl;
            } else if (val == 2) {
                cout << "Forward checking used." << endl;
            } else {
                cout << "Forward checking used with minimum remaining values," << endl;
                cout << "most constraining variable, and least constraining value." << endl;
            }
        }
        // Entered input not a command
        else {
            cout << cmd << " is not a command" << endl;
        }
    } // End while(true)

    return 0;
}
