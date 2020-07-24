cmake_minimum_required(VERSION 3.10)

project(sudoku-solver)

add_executable(sudoku-solver src/main.cpp src/sudoku.cpp src/sudoku_backtrack.cpp)