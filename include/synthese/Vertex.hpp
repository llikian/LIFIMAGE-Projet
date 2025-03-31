/***************************************************************************************************
 * @file  Vertex.hpp
 * @brief Declaration of the Vertex struct
 **************************************************************************************************/

#pragma once

#include "vec.h"

/**
 * @struct Vertex
 * @brief
 */
struct Vertex {
    Vertex(const Point& position, const Vector& normal);

    Point position;
    Vector normal;
    // Point texCoords;
};
