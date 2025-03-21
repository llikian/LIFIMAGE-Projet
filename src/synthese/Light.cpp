/***************************************************************************************************
* @file  Light.cpp
 * @brief Implementation of the Light struct
 **************************************************************************************************/

#include "synthese/Light.hpp"

Light::Light(const Point& position, const Color& color) : position(position), color(color) { }

Light::Light(const Point& position) : position(position), color(1.0f, 1.0f, 1.0f) { }