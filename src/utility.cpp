/***************************************************************************************************
 * @file  utility.cpp
 * @brief 
 **************************************************************************************************/

#include "utility.hpp"

#include <random>
#include <stb_image_write.h>

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

bool operator==(const Color& first, const Color& second) {
    return first.r == second.r && first.g == second.g && first.b == second.b;
}

bool isValueSimilar(float value, float base, float epsilon) {
    return value >= base - epsilon && value <= base + epsilon;
}

bool isColorSimilar(const Color& color, const Color& base, const Color& epsilon) {
    return isValueSimilar(color.r, base.r, epsilon.r)
           && isValueSimilar(color.g, base.g, epsilon.g)
           && isValueSimilar(color.b, base.b, epsilon.b);
}

void write_boolean_array_as_grayscale_image(const std::string& path, const Array2D<bool>& data) {
    std::vector<unsigned char> temp;

    for(unsigned int i = 0 ; i < data.columns ; ++i) {
        for(unsigned int j = 0 ; j < data.rows ; ++j) {
            temp.push_back(255 * data(j, i));
        }
    }

    stbi_flip_vertically_on_write(false);
    stbi_write_png(path.c_str(), data.rows, data.columns, 1, temp.data(), 0);
}

float length(const vec2& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y);
}

float length(const vec3& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}

float length(const vec4& vec) {
    return std::sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w);
}

bool operator!=(const Vector& vec, const Vector& other) {
    return vec.x != other.x || vec.y != other.y || vec.z != other.z;
}

Color hueToRGBA(unsigned short hue) {
    float x = 1.0f - fabsf(fmodf(static_cast<float>(hue) / 60.0f, 2.0f) - 1.0f);

    switch((hue / 60) % 6) {
        case 0: return Color(1.0f, x, 0.0f);
        case 1: return Color(x, 1.0f, 0.0f);
        case 2: return Color(0.0f, 1.0f, x);
        case 3: return Color(0.0f, x, 1.0f);
        case 4: return Color(x, 0.0f, 1.0f);
        case 5: return Color(1.0f, 0.0f, x);
        default: return Color();
    }
}
