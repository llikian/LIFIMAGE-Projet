/***************************************************************************************************
 * @file  analyse.cpp
 * @brief Contains the main program of the project
 **************************************************************************************************/

#include <filesystem>
#include <iostream>
#include <bits/fs_ops.h>

#include "image_io.h"

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
        Square,
        Cross
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

    int pixelAmount = 0;

    Color background;
    for(int i = 1100 ; i < width ; ++i) {
        for(int j = 0 ; j < 650 ; ++j) {
            background = background + puzzle(i, j);
            ++pixelAmount;
        }
    }

    background = background / pixelAmount;

    Color epsilon(0.2f, 0.1f, 0.04f);

    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < height ; ++j) {
            Color& pixel = puzzle(i, j);
            pixel = Color(isColorSimilar(pixel, background, epsilon));
        }
    }

    puzzle = dilate(puzzle, MathematicalMorphology::Cross);
    puzzle = dilate(puzzle, MathematicalMorphology::Cross);
    puzzle = erode(puzzle, MathematicalMorphology::Cross);
    puzzle = erode(puzzle, MathematicalMorphology::Cross);
    puzzle = erode(puzzle, MathematicalMorphology::Cross);
    puzzle = erode(puzzle, MathematicalMorphology::Cross);
    puzzle = erode(puzzle, MathematicalMorphology::Cross);
    puzzle = erode(puzzle, MathematicalMorphology::Cross);
    puzzle = dilate(puzzle, MathematicalMorphology::Square);
    puzzle = dilate(puzzle, MathematicalMorphology::Square);
    puzzle = dilate(puzzle, MathematicalMorphology::Square);
    puzzle = dilate(puzzle, MathematicalMorphology::Square);

    for(int i = 0 ; i < width ; ++i) {
        for(int j = 0 ; j < 10 ; ++j) { puzzle(i, j) = White(); }
        for(int j = height - 10 ; j < height ; ++j) { puzzle(i, j) = White(); }
    }

    for(int j = 0 ; j < height ; ++j) {
        for(int i = 0 ; i < 10 ; ++i) { puzzle(i, j) = White(); }
        for(int i = width - 10 ; i < width ; ++i) { puzzle(i, j) = White(); }
    }

    for(int i = 1180 ; i <= 1250 ; ++i) {
        for(int j = 650 ; j <= 700 ; ++j) {
            puzzle(i, j) = White();
        }
    }

    write_image_png(puzzle, "data/processed.png", false);

    // Image puzzle_original = read_image("data/puzzle.jpg", false);
    // Image pieces(puzzle);
    //
    // for(int i = 0 ; i < width ; ++i) {
    //     for(int j = 0 ; j < height ; ++j) {
    //         if(puzzle(i, j) == Black()) {
    //             pieces(i, j) = puzzle_original(i, j);
    //         }
    //     }
    // }
    //
    // write_image_png(srgb(pieces), "data/only_pieces.png", false);

    return 0;
}
