/***************************************************************************************************
 * @file  analyse.cpp
 * @brief Contains the main program of the project
 **************************************************************************************************/

#include <iostream>
#include <unordered_set>
#include "Array2D.hpp"
#include "image_io.h"
#include "MathematicalMorphology.hpp"
#include "stb_image_write.h"
#include "utility.hpp"

int main() {
    Image puzzle = read_image("data/puzzle.jpg", false);
    int width = puzzle.width();
    int height = puzzle.height();

    /* ---- Get Background Color ---- */
    int pixelAmount = 0;

    Color background;
    for(int i = 1100 ; i < width ; ++i) {
        for(int j = 0 ; j < 650 ; ++j) {
            background = background + puzzle(i, j);
            ++pixelAmount;
        }
    }

    background = background / pixelAmount;

    /* ---- Thresholding ---- */
    Array2D<bool> binaryMask(width, height);
    Color epsilon(0.2f, 0.1f, 0.04f);

    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < height ; ++j) {
            binaryMask(i, j) = isColorSimilar(puzzle(i, j), background, epsilon);
        }
    }


    /* ---- Erase Little Bits and Fill Holes ---- */
    binaryMask = dilate(binaryMask, MathematicalMorphology::Square);
    binaryMask = dilate(binaryMask, MathematicalMorphology::Square);
    binaryMask = dilate(binaryMask, MathematicalMorphology::Cross);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Cross);
    binaryMask = dilate(binaryMask, MathematicalMorphology::Square);

    /* ---- Erase the Borders of the Image and the Logo ---- */
    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < 20 ; ++j) { binaryMask(i, j) = 1; }
        for(int j = height - 20 ; j < height ; ++j) { binaryMask(i, j) = 1; }
    }

    for(int j = 0 ; j < height ; ++j) {
        for(int i = 0 ; i < 20 ; ++i) { binaryMask(i, j) = 1; }
        for(int i = width - 20 ; i < width ; ++i) { binaryMask(i, j) = 1; }
    }

    for(int i = 1180 ; i <= 1250 ; ++i) {
        for(int j = 650 ; j <= 700 ; ++j) {
            binaryMask(i, j) = 1;
        }
    }

    write_boolean_array_as_grayscale_image("data/binary-mask.png", binaryMask);

    /* Applies the Binary Mask to the Original Image and Writes the Result */
    // Image puzzle_original = read_image("data/puzzle.jpg", false);
    // Image only_pieces(puzzle);
    //
    // for(int i = 0 ; i < width ; ++i) {
    //     for(int j = 0 ; j < height ; ++j) {
    //         if(puzzle(i, j) == Black()) {
    //             only_pieces(i, j) = puzzle_original(i, j);
    //         }
    //     }
    // }
    //
    // write_image_png(srgb(only_pieces), "data/only_pieces.png", false);

    /* ---- Labeling ---- */
    Array2D<int> labels(width, height);
    std::unordered_map<int, int> closures;
    int maxLabel = 0;

    static constexpr std::pair<int, int> OFFSETS[]{ { -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 } };

    // First Pass : Initial Labeling
    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < height ; ++j) {
            if(binaryMask(i, j)) { continue; }

            std::vector<int> foundLabels;

            for(auto [offsetX, offsetY] : OFFSETS) {
                int x = i + offsetX;
                int y = j + offsetY;

                if(x < 0 || y < 0 || x >= width) { continue; }

                int value = labels(x, y);
                if(value > 0) {
                    foundLabels.emplace_back(value);
                }
            }

            if(foundLabels.empty()) {
                labels(i, j) = maxLabel++;
            } else {
                int min = foundLabels.at(0);
                for(unsigned int k = 1 ; k < foundLabels.size() ; ++k) {
                    if(foundLabels.at(k) < min) {
                        min = foundLabels.at(k);
                    }
                }

                labels(i, j) = min;

                for(int label : foundLabels) {
                    if(label != min) {
                        closures[label] = min;
                    }
                }
            }
        }
    }

    // Second Pass : Transitive Closures
    std::unordered_map<int, Color> labelColors;
    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < height ; ++j) {
            int& label = labels(i, j);

            int closure = label;

            while(closures.contains(closure)) {
                closure = closures.at(closure);
            }

            label = closure;
            if(!labelColors.contains(label)) {
                labelColors.emplace(label, Color(random(0.0f, 1.0f), random(0.0f, 1.0f), random(0.0f, 1.0f)));
            }
        }
    }

    std::cout << "There are " << labelColors.size() << " unique labels \n";

    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < height ; ++j) {
            if(!binaryMask(i, j)) {
                puzzle(i, j) = labelColors.at(labels(i, j));
            } else {
                puzzle(i, j) = Black();
            }
        }
    }

    write_image_png(srgb(puzzle), "data/labels.png", false);

    return 0;
}
