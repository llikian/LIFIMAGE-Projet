/***************************************************************************************************
 * @file  analyse.cpp
 * @brief Contains the main program of the project
 **************************************************************************************************/

#include <iostream>
#include <unordered_set>
#include "Array2D.hpp"
#include "image_io.h"
#include "MathematicalMorphology.hpp"
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

    /* ---- Labeling ---- */
    Array2D<int> labels(width, height);
    std::unordered_map<int, int> closures;
    int maxLabel = 0;

    static constexpr std::pair<int, int> OFFSETS[]{ { -1, -1 }, { 0, -1 }, { 1, -1 }, { -1, 0 } };
    int neighborLabels[4];
    unsigned int neighbors;

    // First Pass : Initial Labeling
    for(int j = 0 ; j < height ; ++j) {
        for(int i = 0 ; i < width ; ++i) {
            if(binaryMask(i, j)) { continue; }

            neighbors = 0;

            for(auto [offsetX, offsetY] : OFFSETS) {
                unsigned int x = i + offsetX;
                unsigned int y = j + offsetY;

                if(x >= width || y >= height) { continue; }

                int value = labels(x, y);
                if(value > 0) {
                    neighborLabels[neighbors++] = value;
                }
            }

            if(neighbors == 0) {
                labels(i, j) = maxLabel++;
            } else if(neighbors == 1) {
                labels(i, j) = neighborLabels[0];
            } else {
                int min = neighborLabels[0];
                for(unsigned int k = 1 ; k < neighbors ; ++k) {
                    if(neighborLabels[k] < min) {
                        min = neighborLabels[k];
                    }
                }

                labels(i, j) = min;

                for(unsigned int k = 0 ; k < neighbors ; ++k) {
                    if(neighborLabels[k] != min) {
                        closures[neighborLabels[k]] = min;
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
