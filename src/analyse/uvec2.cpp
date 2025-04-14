/***************************************************************************************************
 * @file  uvec2.cpp
 * @brief Implementation of the uvec2 struct
 **************************************************************************************************/

#include "analyse/uvec2.hpp"

uvec2::uvec2() : x(0), y(0) { }

uvec2::uvec2(unsigned int x, unsigned int y) : x(x), y(y) { }

bool uvec2::operator!=(const uvec2& a) const {
    return x != a.x || y != a.y;
}

bool uvec2::operator==(const uvec2& a) const {
    return x == a.x && y == a.y;
}

