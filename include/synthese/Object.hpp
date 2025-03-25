/***************************************************************************************************
 * @file  Object.hpp
 * @brief Declaration of the Object struct
 **************************************************************************************************/

#pragma once

#include <functional>
#include <vector>
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
    explicit Object(const std::function<Color(const Point&)>& getColor);
    virtual ~Object() = default;

    virtual Hit intersect(const Ray& ray) const = 0;

    std::function<Color(const Point&)> getColor;
};

Hit getClosestHit(const Ray& ray, const std::vector<const Object*>& objects);

struct Plane : Object {
    Plane(const Color& color, const Point& point, const Vector& normal);
    Plane(const std::function<Color(const Point&)>& getColor, const Point& point, const Vector& normal);

    Hit intersect(const Ray& ray) const override;

    Point point;
    Vector normal;
};

struct Sphere : Object {
    Sphere(const Color& color, const Point& center, float radius);
    Sphere(const std::function<Color(const Point&)>& getColor, const Point& center, float radius);

    Hit intersect(const Ray& ray) const override;

    Point center;
    float radius;
};

struct Triangle : Object {
    Triangle(const Color& color, const Point& A, const Point& B, const Point& C);
    Triangle(const std::function<Color(const Point&)>& getColor, const Point& A, const Point& B, const Point& C);

    Hit intersect(const Ray& ray) const override;

    Point A;
    Point B;
    Point C;
};
