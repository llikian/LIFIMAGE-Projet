/***************************************************************************************************
 * @file  Ray.hpp
 * @brief Declaration of the Ray struct
 **************************************************************************************************/

#pragma once

#include "Hit.hpp"
#include "vec.h"

/**
 * @struct Ray
 * @brief
 */
struct Ray {
    Ray();
    Ray(const Point& origin, const Vector& direction);

    Point origin;
    Vector direction;

    Point getPoint(float t) const;
    Point getEpsilonPoint(const Hit& hit, float epsilon = 10e-3) const;
};
