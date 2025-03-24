/***************************************************************************************************
 * @file  Object.cpp
 * @brief Implementation of the Object struct
 **************************************************************************************************/

#include "synthese/Object.hpp"

#include <cmath>

Object::Object(const Color& color)
    : getColor([color](const Point&) { return color; }) { }

Object::Object(const std::function<Color(const Point&)>& getColor)
    : getColor(getColor) { }

Plane::Plane(const Color& color, const Point& point, const Vector& normal)
    : Object(color), point(point), normal(normalize(normal)) { }

Plane::Plane(const std::function<Color(const Point&)>& getColor, const Point& point, const Vector& normal)
    : Object(getColor), point(point), normal(normalize(normal)) { }

Hit Plane::intersect(const Ray& ray) const {
    Hit hit;

    hit.intersection = dot(normal, point - ray.origin) / dot(normal, ray.direction);

    if(hit.intersection < 0.0f) {
        hit.intersection = infinity;
    }

    hit.normal = normal;

    return hit;
}

Sphere::Sphere(const Color& color, const Point& center, float radius)
    : Object(color), center(center), radius(radius) { }

Sphere::Sphere(const std::function<Color(const Point&)>& getColor, const Point& center, float radius)
    : Object(getColor), center(center), radius(radius) { }

Hit Sphere::intersect(const Ray& ray) const {
    Hit hit;

    Vector co(center, ray.origin);

    float a = dot(ray.direction, ray.direction);
    float b = 2.0f * dot(ray.direction, co);
    float c = dot(co, co) - radius * radius;

    float delta = std::sqrt(b * b - 4.0f * a * c);

    if(delta > 0.0f) {
        hit.intersection = infinity;

        float x1 = (-b + delta) / 2.0f * a;
        float x2 = (-b - delta) / 2.0f * a;

        if(x1 >= 0.0f) { hit.intersection = x1; }
        if(x2 >= 0.0f && x2 < hit.intersection) { hit.intersection = x2; }
        if(hit.intersection == infinity) { return hit; }
    } else if(delta == 0.0f) {
        hit.intersection = -b / 2.0f * a;
    } else {
        hit.intersection = infinity;
        return hit;
    }

    hit.normal = normalize(ray.getPoint(hit.intersection) - center);

    return hit;
}

Triangle::Triangle(const Color& color, const Point& A, const Point& B, const Point& C)
    : Object(color), A(A), B(B), C(C) { }

Triangle::Triangle(const std::function<Color(const Point&)>& getColor, const Point& A, const Point& B, const Point& C)
    : Object(getColor), A(A), B(B), C(C) { }

Hit Triangle::intersect(const Ray& ray) const {
    Hit hit;

    hit.normal = normalize(cross(B - A, C - A));
    hit.intersection = dot(hit.normal, A - ray.origin) / dot(hit.normal, ray.direction);

    if(hit.intersection < 0.0f) {
        hit.intersection = infinity;
    } else {
        Point point = ray.getPoint(hit.intersection);

        if(dot(hit.normal, cross(Vector(A, B), Vector(A, point))) < 0.0f) { hit.intersection = infinity; }
        if(dot(hit.normal, cross(Vector(B, C), Vector(B, point))) < 0.0f) { hit.intersection = infinity; }
        if(dot(hit.normal, cross(Vector(C, A), Vector(C, point))) < 0.0f) { hit.intersection = infinity; }
    }

    return hit;
}
