/***************************************************************************************************
 * @file  Hit.hpp
 * @brief Declaration of the Hit struct
 **************************************************************************************************/

#pragma once

#include <limits>
#include "vec.h"

struct Object; // Forward-Declared to avoid circular dependency.

static constexpr float infinity = std::numeric_limits<float>::infinity();

/**
 * @struct Hit
 * @brief Stores information about an intersection between a ray and an object.
 */
struct Hit {
    /**
     * @brief Default Constructor. Intersection set to infinity.
     */
    Hit();

    /**
     * @brief Constructor. Creates a hit with a specific intersection and normal.
     * @param intersection The hit's intersection.
     * @param normal The hit's normal.
     */
    Hit(float intersection, const Vector& normal);

    float intersection;   ///< The hit's intersection.
    Vector normal;        ///< The hit's normal.
    const Object* object; ///< A pointer to the hit object.
};
