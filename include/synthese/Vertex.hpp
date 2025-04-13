/***************************************************************************************************
 * @file  Vertex.hpp
 * @brief Declaration of the Vertex struct
 **************************************************************************************************/

#pragma once

#include "vec.h"

/**
 * @struct Vertex
 * @brief Stores vertex attributes.
 */
struct Vertex {
    /**
     * @brief Constructor. Creates a vertex.
     * @param position The vertex's position.
     * @param normal The vertex's normal.
     */
    Vertex(const Point& position, const Vector& normal);

    Point position;  ///< The vertex's position.
    Vector normal;   ///< The vertex's normal.
    // Point texCoords; ///< The vertex's texture coordinates.
};
