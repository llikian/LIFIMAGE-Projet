/***************************************************************************************************
 * @file  Ray.hpp
 * @brief Declaration of the Ray struct
 **************************************************************************************************/

#pragma once

#include "Hit.hpp"
#include "vec.h"

/**
 * @struct Ray
 * @brief Represents a ray starting from a certain point pointing towards a certain direction.
 */
struct Ray {
    /**
     * @brief Constructor. Creates a ray with its origin and direction.
     * @param origin The ray's origin.
     * @param direction The ray's direction.
     */
    Ray(const Point& origin, const Vector& direction);

    /**
     * @brief Calculates a point on the half-line starting from the ray's origin and going in the ray's direction at a
     * specific distance.
     * @param t How far the point is from the origin.
     * @return origin + t * direction.
     */
    Point getPoint(float t) const;

    /**
     * @brief Calculates a point on the half-line starting from the ray's origin and going in the ray's direction at a
     * specific distance and slightly offset along the hit's normal.
     * @param hit The intersection of the ray.
     * @param epsilon How far to go along the hit's normal.
     * @return origin + hit.intersection * direction + epsilon * hit.normal;
     */
    Point getEpsilonPoint(const Hit& hit, float epsilon = 10e-3) const;

    Point origin;     ///< The ray's origin.
    Vector direction; ///< The ray's direction.
};
