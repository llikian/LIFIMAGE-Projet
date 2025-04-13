/***************************************************************************************************
 * @file  Ray.cpp
 * @brief Implementation of the Ray struct
 **************************************************************************************************/

#include "synthese/Ray.hpp"

Ray::Ray(const Point& origin, const Vector& direction) : origin(origin), direction(direction) { }

Point Ray::getPoint(float t) const {
    return origin + t * direction;
}

Point Ray::getEpsilonPoint(const Hit& hit, float epsilon) const {
    return getPoint(hit.intersection) + epsilon * hit.normal;
}
