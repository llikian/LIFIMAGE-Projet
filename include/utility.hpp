/***************************************************************************************************
 * @file  utility.hpp
 * @brief 
 **************************************************************************************************/

#pragma once

#include <string>
#include "Array2D.hpp"
#include "color.h"

float random(float min, float max);
int random(int min, int max);

inline float pow2(float x) {
    return x * x;
}

void operator+=(Color& left, const Color& right);
void operator*=(Color& color, float scalar);
void operator*=(Color& color, const Color& color2);
bool operator==(const Color& first, const Color& second);

bool isValueSimilar(float value, float base, float epsilon);
bool isColorSimilar(const Color& color, const Color& base, const Color& epsilon);

void write_boolean_array_as_grayscale_image(const std::string& path, const Array2D<bool>& data);
