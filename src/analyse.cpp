/***************************************************************************************************
 * @file  analyse.cpp
 * @brief Contains the main program of the project
 **************************************************************************************************/

#include <filesystem>
#include <iostream>
#include <unordered_map>
#include "Array2D.hpp"
#include "image_io.h"
#include "MathematicalMorphology.hpp"
#include "utility.hpp"

struct MinMaxPos {
    unsigned int minX{-1u};
    unsigned int minY{-1u};
    unsigned int maxX{0u};
    unsigned int maxY{0u};
};

int main() {
    Image puzzle = read_image("data/puzzle.jpg", false);
    unsigned int width = puzzle.width();
    unsigned int height = puzzle.height();

    /* ---- Get Background Color ---- */
    unsigned int pixelAmount = 0;

    Color background;
    for(unsigned int j = 0 ; j < 650 ; ++j) {
        for(unsigned int i = 1100 ; i < width ; ++i) {
            background = background + puzzle(i, j);
            ++pixelAmount;
        }
    }

    background = background / pixelAmount;

    /* ---- Thresholding ---- */
    Array2D<bool> binaryMask(width, height);
    Color epsilon(0.2f, 0.1f, 0.04f);

    for(unsigned int j = 0 ; j < height ; ++j) {
        for(unsigned int i = 0 ; i < width ; ++i) {
            binaryMask(i, j) = isColorSimilar(puzzle(i, j), background, epsilon);
        }
    }

    // Erase Little Bits and Fill Holes
    binaryMask = dilate(binaryMask, MathematicalMorphology::Square);
    binaryMask = dilate(binaryMask, MathematicalMorphology::Square);
    binaryMask = dilate(binaryMask, MathematicalMorphology::Cross);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Square);
    binaryMask = erode(binaryMask, MathematicalMorphology::Cross);
    binaryMask = erode(binaryMask, MathematicalMorphology::Cross);
    binaryMask = dilate(binaryMask, MathematicalMorphology::Square);

    // Erase the Borders of the Image and the Logo
    for(unsigned int i = 0 ; i < width ; ++i) {
        for(unsigned int j = 0 ; j < 20 ; ++j) { binaryMask(i, j) = 1; }
        for(unsigned int j = height - 20 ; j < height ; ++j) { binaryMask(i, j) = 1; }
    }

    for(unsigned int j = 0 ; j < height ; ++j) {
        for(unsigned int i = 0 ; i < 20 ; ++i) { binaryMask(i, j) = 1; }
        for(unsigned int i = width - 20 ; i < width ; ++i) { binaryMask(i, j) = 1; }
    }

    for(unsigned int j = 650 ; j <= 700 ; ++j) {
        for(unsigned int i = 1180 ; i <= 1250 ; ++i) {
            binaryMask(i, j) = 1;
        }
    }

    write_boolean_array_as_grayscale_image("data/binary-mask.png", binaryMask);

    /* ---- Labeling ---- */
    Array2D<unsigned int> labels(width, height);
    std::unordered_map<unsigned int, unsigned int> closures;
    unsigned int maxLabel = 1;

    unsigned int neighborLabels[4];
    unsigned int neighbors;
    auto checkOffset = [&width, &height, &labels, &neighborLabels, &neighbors](unsigned int x, unsigned int y) {
        if(x >= width || y >= height) { return; }

        unsigned int value = labels(x, y);
        if(value > 0) {
            neighborLabels[neighbors++] = value;
        }
    };

    // First Pass : Initial Labeling
    for(unsigned int j = 0 ; j < height ; ++j) {
        for(unsigned int i = 0 ; i < width ; ++i) {
            if(binaryMask(i, j)) { continue; }

            neighbors = 0;

            checkOffset(i - 1, j - 1);
            checkOffset(i, j - 1);
            checkOffset(i + 1, j - 1);
            checkOffset(i - 1, j);

            if(neighbors == 0) {
                labels(i, j) = maxLabel++;
            } else if(neighbors == 1) {
                labels(i, j) = neighborLabels[0];
            } else {
                unsigned int min = neighborLabels[0];
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
    std::unordered_map<unsigned int, Color> labelColors;
    std::unordered_map<unsigned int, MinMaxPos> labelPositions;

    for(unsigned int j = 0 ; j < height ; ++j) {
        for(unsigned int i = 0 ; i < width ; ++i) {
            unsigned int& label = labels(i, j);

            unsigned int closure = label;

            while(closures.contains(closure)) {
                closure = closures.at(closure);
            }

            label = closure;
            if(label > 0) {
                if(i < labelPositions[label].minX) { labelPositions[label].minX = i; }
                if(j < labelPositions[label].minY) { labelPositions[label].minY = j; }
                if(i > labelPositions[label].maxX) { labelPositions[label].maxX = i; }
                if(j > labelPositions[label].maxY) { labelPositions[label].maxY = j; }

                if(!labelColors.contains(label)) {
                    labelColors.emplace(label, Color(random(0.0f, 1.0f), random(0.0f, 1.0f), random(0.0f, 1.0f)));
                }
            }
        }
    }

    std::cout << "There are " << labelColors.size() << " unique labels:\n";
    for(const auto& [label, _] : labelColors) {
        std::cout << label << ' ';
    }
    std::cout << '\n';

    Image labels_img(width, height);
    for(unsigned int j = 0 ; j < height ; ++j) {
        for(unsigned int i = 0 ; i < width ; ++i) {
            labels_img(i, j) = binaryMask(i, j) ? Black() : labelColors.at(labels(i, j));
        }
    }

    write_image_png(srgb(labels_img), "data/labels.png", false);

    /* ---- Extract Pieces ---- */
    std::filesystem::create_directory("data/pieces");
    std::filesystem::create_directory("data/piece-masks");

    for(const auto& [label, position] : labelPositions) {
        unsigned int padding = 10;
        unsigned int w = position.maxX - position.minX;
        unsigned int h = position.maxY - position.minY;

        Image piece(w + 2 * padding, h + 2 * padding, background);
        Image piece_mask(w + 2 * padding, h + 2 * padding, White());

        for(unsigned int j = 0 ; j < h ; ++j) {
            for(unsigned int i = 0 ; i < w ; ++i) {
                piece(i + padding, j + padding) = puzzle(i + position.minX, j + position.minY);
                piece_mask(i + padding, j + padding) = Color(binaryMask(i + position.minX, j + position.minY));
            }
        }

        write_image_png(srgb(piece), ("data/pieces/piece-" + std::to_string(label) + ".png").c_str(), false);
        write_image_png(srgb(piece_mask), ("data/piece-masks/piece-mask-" + std::to_string(label) + ".png").c_str(), false);
    }

    return 0;
}
