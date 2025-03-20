/***************************************************************************************************
* @file  Light.cpp
 * @brief Implementation of the Light struct
 **************************************************************************************************/

#include "synthese/Light.hpp"

Light::Light(const Vector& direction, const Color& color) : direction(direction), color(color) { }

Light::Light(const Vector& direction) : direction(direction), color(1.0f, 1.0f, 1.0f) { }