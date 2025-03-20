/***************************************************************************************************
 * @file  Hit.cpp
 * @brief Implementation of the Hit struct
 **************************************************************************************************/

#include "synthese/Hit.hpp"

Hit::Hit() : intersection(infinity), normal(0.0f, 0.0f, 0.0f), object(nullptr) { }

Hit::Hit(float intersection, const Vector& normal) : intersection(intersection), normal(normal), object(nullptr) { }
