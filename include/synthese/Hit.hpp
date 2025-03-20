/***************************************************************************************************
 * @file  Hit.hpp
 * @brief Declaration of the Hit struct
 **************************************************************************************************/

#pragma once

#include <limits>
#include "vec.h"

static constexpr float infinity = std::numeric_limits<float>::infinity();

/**
 * @struct Hit
 * @brief
 */
struct Hit {
    Hit();
    Hit(float intersection, const Vector& normal);

    float intersection;
    Vector normal;
};