/***************************************************************************************************
 * @file  Vertex.cpp
 * @brief Implementation of the Vertex struct
 **************************************************************************************************/

#include "synthese/Vertex.hpp"

Vertex::Vertex(const Point& position, const Vector& normal)
    : position(position), normal(normal) { }