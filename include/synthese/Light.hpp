/***************************************************************************************************
* @file  Light.hpp
 * @brief Declaration of the Light struct
 **************************************************************************************************/

#pragma once

#include "color.h"
#include "Ray.hpp"
#include "vec.h"

struct Light {
    explicit Light(const Color& color);
    virtual ~Light() = default;

    virtual Color calculate(const Hit& hit, Ray& ray) const = 0;

    Color color;
};

struct DirectionalLight : Light {
    DirectionalLight(const Color& color, const Vector& direction);

    Color calculate(const Hit& hit, Ray& ray) const override;

    Vector direction;
};

struct PointLight : Light {
    PointLight(const Color& color, const Point& position, float radius);

    Color calculate(const Hit& hit, Ray& ray) const override;

    Point position;
    float radius;
};
