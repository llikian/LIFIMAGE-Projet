/***************************************************************************************************
 * @file  analyse.cpp
 * @brief Contains the main program of the project
 **************************************************************************************************/

#include <iostream>
#include <unordered_set>
#include "image_io.h"
#include "utility.hpp"

bool operator==(const Color& first, const Color& second) {
    return first.r == second.r && first.g == second.g && first.b == second.b;
}

bool isValueSimilar(float value, float base, float epsilon) {
    return value >= base - epsilon && value <= base + epsilon;
}

bool isColorSimilar(const Color& color, const Color& base, const Color& epsilon) {
    return isValueSimilar(color.r, base.r, epsilon.r)
           && isValueSimilar(color.g, base.g, epsilon.g)
           && isValueSimilar(color.b, base.b, epsilon.b);
}

namespace MathematicalMorphology {
    enum StructuringElement : bool {
        Square = true,
        Cross  = false
    };

    int applyStructuringElement(const Image& image, int x, int y, StructuringElement structuringElement) {
        const std::pair<int, int>* offsets;
        int neighbors;

        if(structuringElement) { // Square
            static constexpr std::pair<int, int> OFFSETS[]{
                { -1, -1 }, { -1, 0 }, { -1, 1 },
                { 0, -1 }, { 0, 0 }, { 0, 1 },
                { 1, -1 }, { 1, 0 }, { 1, 1 },
            };

            offsets = OFFSETS;
            neighbors = 9;
        } else { // Cross
            static constexpr std::pair<int, int> OFFSETS[]{
                { -1, 0 }, { 0, -1 }, { 0, 0 }, { 0, 1 }, { 1, 0 }
            };

            offsets = OFFSETS;
            neighbors = 4;
        }

        int n = 0;

        for(int i = 0 ; i < neighbors ; ++i) {
            if(image(x + offsets[i].first, y + offsets[i].second) == Black()) {
                ++n;
            }
        }

        return n;
    }

    Image dilate(const Image& image, StructuringElement structuringElement) {
        Image result(image);

        for(int i = 0 ; i < image.width() ; ++i) {
            for(int j = 0 ; j < image.height() ; ++j) {
                if(applyStructuringElement(image, i, j, structuringElement) > 0) {
                    result(i, j) = Black();
                }
            }
        }

        return result;
    }

    Image erode(const Image& image, StructuringElement structuringElement) {
        Image result(image);

        int neighbors = structuringElement ? 9 : 4;

        for(int i = 0 ; i < image.width() ; ++i) {
            for(int j = 0 ; j < image.height() ; ++j) {
                if(applyStructuringElement(image, i, j, structuringElement) < neighbors) {
                    result(i, j) = White();
                }
            }
        }

        return result;
    }
}

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
    Color epsilon(0.2f, 0.1f, 0.04f);

    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < height ; ++j) {
            Color& pixel = puzzle(i, j);
            pixel = Color(isColorSimilar(pixel, background, epsilon));
        }
    }

    /* ---- Erase Little Bits and Fill Holes ---- */
    puzzle = dilate(puzzle, MathematicalMorphology::Square);
    puzzle = dilate(puzzle, MathematicalMorphology::Square);
    puzzle = dilate(puzzle, MathematicalMorphology::Cross);
    puzzle = erode(puzzle, MathematicalMorphology::Square);
    puzzle = erode(puzzle, MathematicalMorphology::Square);
    puzzle = erode(puzzle, MathematicalMorphology::Square);
    puzzle = erode(puzzle, MathematicalMorphology::Square);
    puzzle = erode(puzzle, MathematicalMorphology::Square);
    puzzle = erode(puzzle, MathematicalMorphology::Cross);
    puzzle = dilate(puzzle, MathematicalMorphology::Square);

    /* ---- Erase the Borders of the Image and the Logo ---- */
    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < 20 ; ++j) { puzzle(i, j) = White(); }
        for(int j = height - 20 ; j < height ; ++j) { puzzle(i, j) = White(); }
    }

    for(int j = 0 ; j < height ; ++j) {
        for(int i = 0 ; i < 20 ; ++i) { puzzle(i, j) = White(); }
        for(int i = width - 20 ; i < width ; ++i) { puzzle(i, j) = White(); }
    }

    for(int i = 1180 ; i <= 1250 ; ++i) {
        for(int j = 650 ; j <= 700 ; ++j) {
            puzzle(i, j) = White();
        }
    }

    write_image_png(puzzle, "data/binary-mask.png", false);

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
    std::vector<int> labels(width * height);
    std::unordered_map<int, int> closures;
    int maxLabel = 0;

    auto index = [&width](int x, int y) { return y * width + x; };

    static constexpr std::pair<int, int> OFFSETS[]{ { -1, -1 }, { -1, 0 }, { -1, 1 }, { 0, -1 } };

    // First Pass : Initial Labeling
    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < height ; ++j) {
            if(puzzle(i, j).r == 1.0f) { continue; }

            std::vector<int> foundLabels;

            for(auto [offsetX, offsetY] : OFFSETS) {
                int x = i + offsetX;
                int y = j + offsetY;

                if(x < 0 || y < 0 || x >= width) { continue; }

                int value = labels[index(x, y)];
                if(value > 0) {
                    foundLabels.emplace_back(value);
                }
            }

            if(foundLabels.empty()) {
                labels[index(i, j)] = maxLabel++;
            } else {
                int min = foundLabels.at(0);
                for(unsigned int k = 1 ; k < foundLabels.size() ; ++k) {
                    if(foundLabels.at(k) < min) {
                        min = foundLabels.at(k);
                    }
                }

                labels[index(i, j)] = min;

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
    for(int& label : labels) {
        int closure = label;

        while(closures.contains(closure)) {
            closure = closures.at(closure);
        }

        label = closure;
        if(!labelColors.contains(label)) {
            labelColors.emplace(label, Color(random(0.0f, 1.0f), random(0.0f, 1.0f), random(0.0f, 1.0f)));
        }
    }

    std::cout << "There are " << labelColors.size() << " unique labels \n";

    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < height ; ++j) {
            const Color& pixel = puzzle(i, j);
            if(pixel.r == 0.0f) {
                puzzle(i, j) = labelColors.at(labels.at(index(i, j)));
            } else {
                puzzle(i, j) = Black();
            }
        }
    }

    write_image_png(srgb(puzzle), "data/labels.png", false);

    return 0;
}
