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
#include "Vertex.hpp"

using ColorFunc = std::function<Color(const Point&)>;

enum class ObjectType : unsigned char {
    Plane,
    Sphere,
    Triangle,
    MeshTriangle,
    TYPE_COUNT
};

/**
 * @struct Object
 * @brief
 */
struct Object {
    explicit Object(const Color& color);
    explicit Object(const ColorFunc& getColor);
    virtual ~Object() = default;

    virtual ObjectType getType() const = 0;

    virtual Hit intersect(const Ray& ray) const = 0;

    virtual Point getCentroid() const = 0;
    virtual void compareBoundingBox(Point& pmin, Point& pmax) const = 0;

    ColorFunc getColor;
};

Hit getClosestHit(const Ray& ray, const std::vector<const Object*>& objects);

struct Plane : Object {
    Plane(const Color& color, const Point& point, const Vector& normal);
    Plane(const ColorFunc& getColor, const Point& point, const Vector& normal);

    ObjectType getType() const override;

    Hit intersect(const Ray& ray) const override;

    /* These functions don't make sense for a plane but they need to be implemented since they are
     * pure virtual in Object */
    Point getCentroid() const override;
    void compareBoundingBox(Point& pmin, Point& pmax) const override;

    Point point;
    Vector normal;
};

struct Sphere : Object {
    Sphere(const Color& color, const Point& center, float radius);
    Sphere(const ColorFunc& getColor, const Point& center, float radius);

    ObjectType getType() const override;

    Hit intersect(const Ray& ray) const override;

    Point getCentroid() const override;
    void compareBoundingBox(Point& pmin, Point& pmax) const override;

    Point center;
    float radius;
};

struct Triangle : Object {
    Triangle(const Color& color, const Point& A, const Point& B, const Point& C);
    Triangle(const ColorFunc& getColor, const Point& A, const Point& B, const Point& C);

    ObjectType getType() const override;

    Hit intersect(const Ray& ray) const override;

    Point getCentroid() const override;
    void compareBoundingBox(Point& pmin, Point& pmax) const override;

    Point A;
    Point B;
    Point C;
};

struct MeshTriangle : Object {
    MeshTriangle(const Color& color, const Vertex& A, const Vertex& B, const Vertex& C);
    MeshTriangle(const ColorFunc& getColor, const Vertex& A, const Vertex& B, const Vertex& C);

    ObjectType getType() const override;

    Hit intersect(const Ray& ray) const override;

    Point getCentroid() const override;
    void compareBoundingBox(Point& pmin, Point& pmax) const override;

    Vertex A;
    Vertex B;
    Vertex C;
};
