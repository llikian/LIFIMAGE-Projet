/***************************************************************************************************
 * @file  utility.hpp
 * @brief 
 **************************************************************************************************/

#pragma once

#include "color.h"

float random(float min, float max);
int random(int min, int max);

inline float pow2(float x) {
    return x * x;
}

void operator+=(Color& left, const Color& right);
void operator*=(Color& color, float scalar);
void operator*=(Color& color, const Color& color2);
