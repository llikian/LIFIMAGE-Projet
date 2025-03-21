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
    Light(const Point& position, const Color& color);
    Light(const Point& position);

    Point position;
    Color color;
};