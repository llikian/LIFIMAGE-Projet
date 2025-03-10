/***************************************************************************************************
 * @file  analyse.cpp
 * @brief Contains the main program of the project
 **************************************************************************************************/

#include <iostream>
#include "image_io.h"

int main() {
    Image puzzle = read_image("data/puzzle.jpg");



    write_image(puzzle, "data/processed_puzzle.jpg");

    return 0;
}