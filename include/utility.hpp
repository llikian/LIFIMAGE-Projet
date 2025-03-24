/***************************************************************************************************
 * @file  utility.hpp
 * @brief 
 **************************************************************************************************/

#pragma once

#include <cmath>
#include <string>
#include <vec.h>

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

template<typename Type>
Color lerp(const Type& A, const Type& B, float t) {
    return A * (1.0f - t) + B * t;
}

float length(const vec2& vec);
float length(const vec3& vec);
float length(const vec4& vec);

inline float radians(float deg) {
    return deg * M_PIf / 180.0f;
}

inline float degrees(float rad) {
    return rad * 180.0f / M_PIf;
}

bool operator!=(const Vector& vec, const Vector& other);