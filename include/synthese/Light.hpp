/***************************************************************************************************
* @file  Light.hpp
 * @brief Declaration of the Light struct
 **************************************************************************************************/

#pragma once

#include <vector>
#include "color.h"
#include "Object.hpp"
#include "Ray.hpp"
#include "vec.h"

struct Light {
    explicit Light(const Color& color);
    virtual ~Light() = default;

    virtual Color calculate(const Hit& hit, const Point& point, const std::vector<const Object*>& objects) const = 0;
    static bool isInShadow(const Object* object, const Ray& ray, const std::vector<const Object*>& objects);

    Color color;
};

struct DirectionalLight : Light {
    DirectionalLight(const Color& color, const Vector& direction);

    Color calculate(const Hit& hit, const Point& point, const std::vector<const Object*>& objects) const override;

    Vector direction;
};

struct PointLight : Light {
    PointLight(const Color& color, const Point& position, float radius);

    Color calculate(const Hit& hit, const Point& point, const std::vector<const Object*>& objects) const override;

    Point position;
    float radius;
};
