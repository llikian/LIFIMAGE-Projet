/***************************************************************************************************
 * @file  Object.cpp
 * @brief Implementation of the Object struct
 **************************************************************************************************/

#include "synthese/Object.hpp"

#include <cmath>

Object::Object(const Color& color)
    : getColor([color](const Point&) { return color; }) { }

Object::Object(const ColorFunc& getColor)
    : getColor(getColor) { }

Hit getClosestHit(const Ray& ray, const std::vector<const Object*>& objects) {
    Hit closest;

    for(const Object* object : objects) {
        Hit hit = object->intersect(ray);

        if(hit.intersection != infinity && (closest.object == nullptr || hit.intersection < closest.intersection)) {
            closest.intersection = hit.intersection;
            closest.normal = hit.normal;
            closest.object = object;
        }
    }

    return closest;
}

const Object* getClosestObject(const Ray& ray, const std::vector<const Object*>& objects) {
    float intersection = infinity;
    const Object* obj = nullptr;

    for(const Object* object : objects) {
        Hit hit = object->intersect(ray);

        if(hit.intersection != infinity && (obj == nullptr || hit.intersection < intersection)) {
            intersection = hit.intersection;
            obj = object;
        }
    }

    return obj;
}

Plane::Plane(const Color& color, const Point& point, const Vector& normal)
    : Object(color), point(point), normal(normalize(normal)) { }

Plane::Plane(const ColorFunc& getColor, const Point& point, const Vector& normal)
    : Object(getColor), point(point), normal(normalize(normal)) { }

Hit Plane::intersect(const Ray& ray) const {
    Hit hit;

    hit.intersection = dot(normal, point - ray.origin) / dot(normal, ray.direction);
    if(hit.intersection < 0.0f) { return Hit(); }
    hit.normal = normal;

    return hit;
}

Sphere::Sphere(const Color& color, const Point& center, float radius)
    : Object(color), center(center), radius(radius) { }

Sphere::Sphere(const ColorFunc& getColor, const Point& center, float radius)
    : Object(getColor), center(center), radius(radius) { }

Hit Sphere::intersect(const Ray& ray) const {
    Hit hit;

    Vector co(center, ray.origin);

    // float a = dot(ray.direction, ray.direction); // ray.direction is normalized so this always equals 1
    float b = 2.0f * dot(ray.direction, co);
    float c = length2(co) - radius * radius;

    float delta = std::sqrt(b * b - 4.0f * c);

    if(delta < 0.0f) { return Hit(); }

    float x1 = (-b + delta) / 2.0f;
    float x2 = (-b - delta) / 2.0f;

    if(x1 >= 0.0f) { hit.intersection = x1; }
    if(x2 >= 0.0f && x2 < hit.intersection) { hit.intersection = x2; }
    if(hit.intersection == infinity) { return Hit(); }

    hit.normal = normalize(ray.getPoint(hit.intersection) - center);

    return hit;
}

Triangle::Triangle(const Color& color, const Point& A, const Point& B, const Point& C)
    : Object(color), A(A), B(B), C(C) { }

Triangle::Triangle(const ColorFunc& getColor, const Point& A, const Point& B, const Point& C)
    : Object(getColor), A(A), B(B), C(C) { }

Hit Triangle::intersect(const Ray& ray) const {
    Hit hit;

    hit.normal = normalize(cross(B - A, C - A));
    hit.intersection = dot(hit.normal, A - ray.origin) / dot(hit.normal, ray.direction);

    if(hit.intersection < 0.0f) { return Hit(); }
    Point point = ray.getPoint(hit.intersection);

    if(dot(hit.normal, cross(B - A, point - A)) < 0.0f) { return Hit(); }
    if(dot(hit.normal, cross(C - B, point - B)) < 0.0f) { return Hit(); }
    if(dot(hit.normal, cross(A - C, point - C)) < 0.0f) { return Hit(); }

    return hit;
}

MeshTriangle::MeshTriangle(const Color& color, const Vertex& A, const Vertex& B, const Vertex& C)
    : Object(color), A(A), B(B), C(C) { }

MeshTriangle::MeshTriangle(const ColorFunc& getColor, const Vertex& A, const Vertex& B, const Vertex& C)
    : Object(getColor), A(A), B(B), C(C) { }

Hit MeshTriangle::intersect(const Ray& ray) const {
    Hit hit;

    hit.normal = cross(B.position - A.position, C.position - A.position);
    float area2 = length(hit.normal); // 2 times the area of triangle ABC
    hit.normal = hit.normal / area2;

    hit.intersection = dot(hit.normal, A.position - ray.origin) / dot(hit.normal, ray.direction);

    if(hit.intersection < 0.0f) { return Hit(); }

    Point point = ray.getPoint(hit.intersection);

    Vector BCP = cross(C.position - B.position, point - B.position);
    Vector CAP = cross(A.position - C.position, point - C.position);

    if(dot(hit.normal, cross(B.position - A.position, point - A.position)) < 0.0f) { return Hit(); }
    if(dot(hit.normal, BCP) < 0.0f) { return Hit(); }
    if(dot(hit.normal, CAP) < 0.0f) { return Hit(); }

    float u = length(BCP) / area2;
    float v = length(CAP) / area2;
    float w = 1.0f - u - v;

    hit.normal = normalize(u * A.normal + v * B.normal + w * C.normal);

    return hit;
}
