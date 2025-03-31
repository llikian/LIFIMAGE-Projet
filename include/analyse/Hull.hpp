#pragma once
#include "Array2D.hpp"

struct Hull {
    Array2D<bool> hull;

    Hull(unsigned int rows, unsigned int columns): hull(rows, columns) {}
    Hull(Array2D<bool> hull) : hull(hull) {}

    void QuickHull(Array2D<bool> tab);
};