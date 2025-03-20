/***************************************************************************************************
* @file  Light.hpp
 * @brief Declaration of the Light struct
 **************************************************************************************************/

#pragma once

#include <color.h>
#include <vec.h>

/**
 * @struct Light
 * @brief
 */
struct Light {
    Light(const Vector& direction, const Color& color);
    Light(const Vector& direction);

    Vector direction;
    Color color;
};