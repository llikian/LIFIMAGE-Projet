/***************************************************************************************************
 * @file  utility.cpp
 * @brief 
 **************************************************************************************************/

#include "utility.hpp"

#include <random>

float random(float min, float max) {
    static std::random_device seed;
    static std::default_random_engine generator(seed());

    return std::uniform_real_distribution<float>(min, max)(generator);
}

int random(int min, int max) {
    static std::random_device seed;
    static std::default_random_engine generator(seed());

    return std::uniform_int_distribution<int>(min, max)(generator);
}

void operator+=(Color& left, const Color& right) {
    left.r += right.r;
    left.g += right.g;
    left.b += right.b;
}

void operator*=(Color& color, float scalar) {
    color.r *= scalar;
    color.g *= scalar;
    color.b *= scalar;
}

void operator*=(Color& color, const Color& color2) {
    color.r *= color2.r;
    color.g *= color2.g;
    color.b *= color2.b;
}
