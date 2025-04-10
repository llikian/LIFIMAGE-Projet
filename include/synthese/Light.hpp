/***************************************************************************************************
* @file  Light.hpp
 * @brief Declaration of the Light struct
 **************************************************************************************************/

#pragma once

#include "color.h"
#include "Ray.hpp"
#include "vec.h"

class Scene;

enum class LightType : unsigned char {
    DirectionalLight,
    PointLight,
    TYPE_COUNT
};

struct Light {
    explicit Light(const Color& color);
    virtual ~Light() = default;

    virtual LightType getType() const = 0;

    virtual Color calculate(const Hit& hit, const Point& point, const Scene* scene) const = 0;
    static bool isInShadow(const Object* object, const Ray& ray, const Scene* scene);

    Color color;
};

struct DirectionalLight : Light {
    DirectionalLight(const Color& color, const Vector& direction);

    LightType getType() const override;

    Color calculate(const Hit& hit, const Point& point, const Scene* scene) const override;

    Vector direction;
};

struct PointLight : Light {
    PointLight(const Color& color, const Point& position, float radius);

    LightType getType() const override;

    Color calculate(const Hit& hit, const Point& point, const Scene* scene) const override;

    Point position;
    float radius;
};
