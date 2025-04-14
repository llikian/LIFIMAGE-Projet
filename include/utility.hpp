/***************************************************************************************************
 * @file  utility.hpp
 * @brief 
 **************************************************************************************************/

#pragma once

#include <cmath>
#include <string>
#include <vec.h>

#include "analyse/uvec2.hpp"
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

vec2 operator*(const vec2& vec, float scalar);
vec2 operator*(float scalar, const vec2& vec);

bool isValueSimilar(float value, float base, float epsilon);
bool isColorSimilar(const Color& color, const Color& base, const Color& epsilon);

void write_boolean_array_as_grayscale_image(const std::string& path, const Array2D<bool>& data);

template<typename Type>
Color lerp(const Type& A, const Type& B, float t) {
    return A * (1.0f - t) + B * t;
}

inline float length(const vec2& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

inline float length(const vec3& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

inline float length(const vec4& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

inline float dot(const vec2& vec, const vec2& other) {
    return vec.x * other.x + vec.y * other.y;
}

inline float dot(const vec3& vec, const vec3& other) {
    return vec.x * other.x + vec.y * other.y + vec.z * other.z;
}

inline float dot(const vec4& vec, const vec4& other) {
    return vec.x * other.x + vec.y * other.y + vec.z * other.z + vec.w * other.w;
}

inline float radians(float deg) {
    return deg * M_PIf / 180.0f;
}

inline float degrees(float rad) {
    return rad * 180.0f / M_PIf;
}

bool operator!=(const Vector& vec, const Vector& other);

Color hueToRGBA(unsigned short hue);

inline Point min3(const Point& point, const Point& other) {
    return Point(std::min(point.x, other.x),
                 std::min(point.y, other.y),
                 std::min(point.z, other.z));
}

inline Point max3(const Point& point, const Point& other) {
    return Point(std::max(point.x, other.x),
                 std::max(point.y, other.y),
                 std::max(point.z, other.z));
}

inline float triangle_area(uvec2 a, uvec2 b, uvec2 c) {
    return 0.5f * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
}

inline bool is_in_triangle(uvec2 p, uvec2 a, uvec2 b, uvec2 c) {
    return triangle_area(a, b, c) >= triangle_area(p, b, c) + triangle_area(a, p, c) + triangle_area(a, b, p);
}