#include "analyse/Hull.hpp"

#include <analyse/uvec2.hpp>

void Hull::QuickHull(Array2D<bool> tab) {
    hull = Array2D<bool>(tab.rows, tab.columns);

    uvec2 leftmost = {hull.columns, hull.rows};
    uvec2 rightmost = {0, 0};
    for (unsigned int y = 0; y < tab.columns; ++y) {
        for (unsigned int x = 0; x < tab.rows; ++x) {
            if (tab(x, y) && x < leftmost.x) {
                leftmost = {x, y};
            }
            if (tab(x, y) && x >= rightmost.x) {
                rightmost = {x, y};
            }
        }
    }


}
