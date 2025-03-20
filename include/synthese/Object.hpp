/***************************************************************************************************
 * @file  Object.hpp
 * @brief Declaration of the Object struct
 **************************************************************************************************/

#pragma once

#include "color.h"
#include "Hit.hpp"
#include "Ray.hpp"
#include "vec.h"

/**
 * @struct Object
 * @brief
 */
struct Object {
    explicit Object(const Color& color);
    virtual ~Object() = default;

    virtual Hit intersect(const Ray& ray) const = 0;

    Color color;
};

struct Plane : Object {
    Plane(const Color& color, const Point& point, const Vector& normal);

    Hit intersect(const Ray& ray) const override;

    Point point;
    Vector normal;
};

struct Sphere : Object {
    Sphere(const Color& color, const Point& center, float radius);

    Hit intersect(const Ray& ray) const override;

    Point center;
    float radius;
};

struct Triangle : Object {
    Triangle(const Color& color, const Point& A, const Point& B, const Point& C);

    Hit intersect(const Ray& ray) const override;

    Point A;
    Point B;
    Point C;
};
