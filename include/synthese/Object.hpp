/***************************************************************************************************
 * @file  Object.hpp
 * @brief Declaration of the Object struct
 **************************************************************************************************/

#pragma once

#include <functional>
#include "color.h"
#include "Hit.hpp"
#include "Ray.hpp"
#include "vec.h"
#include "Vertex.hpp"

using ColorFunc = std::function<Color(const Point&)>;

/**
 * @enum ObjectType
 * @brief Enumeration of all Object types.
 */
enum class ObjectType : unsigned char {
    Plane,
    Sphere,
    Triangle,
    MeshTriangle,
    TYPE_COUNT
};

/**
 * @struct Object
 * @brief An object, used to fill up a scene.
 */
struct Object {
    /**
     * @brief Constructor. Creates an object with its color function always returning the same color.
     * @param color The object's color.
     */
    explicit Object(const Color& color);

    /**
     * @brief Constructor. Creates an object with a specific color function.
     * @param getColor The object's color function.
     */
    explicit Object(const ColorFunc& getColor);

    /**
     * @brief Destructor.
     */
    virtual ~Object() = default;

    /**
     * @return The object's type.
     */
    virtual ObjectType getType() const = 0;

    /**
     * @brief Calculates the intersection between a ray and the object.
     * @param ray The ray to calculate the intersection with.
     * @return The information on the hit object. If no object is hit, the intersection will be set to infinity.
     */
    virtual Hit intersect(const Ray& ray) const = 0;

    /**
     * @brief Calculates the centroid (barycenter) of the object.
     * @return The centroid of the object.
     */
    virtual Point getCentroid() const = 0;

    /**
     * @brief Compares this object to a bounding box. Replaces the values of pmin and pmax's components if they are
     * respectively lower or higher.
     * @param pmin The bounding box's current lower bound.
     * @param pmax The bounding box's current higher bound.
     */
    virtual void compareBoundingBox(Point& pmin, Point& pmax) const = 0;

    ColorFunc getColor; ///< The object's color function.
};

/**
 * @struct Plane
 * @brief A plane.
 */
struct Plane : Object {
    /**
     * @brief Constructor. Creates a plane with a plain color.
     * @param color The plane's color.
     * @param point A point on the plane.
     * @param normal The plane's normal
     */
    Plane(const Color& color, const Point& point, const Vector& normal);

    /**
     * @brief Constructor. Creates a plane with a specific color function.
     * @param getColor The plane's color function.
     * @param point A point on the plane.
     * @param normal The plane's normal.
     */
    Plane(const ColorFunc& getColor, const Point& point, const Vector& normal);

    /**
     * @return The object's type.
     */
    ObjectType getType() const override;

    /**
     * @brief Calculates the intersection between a ray and the plane.
     * @param ray The ray to calculate the intersection with.
     * @return The information on the hit object. If no object is hit, the intersection will be set to infinity.
     */
    Hit intersect(const Ray& ray) const override;

    /**
     * @brief This function doesn't really make sense for a plane but needs to be implemented since it's a pure virtual
     * function of Object.
     * @return The point on the plane.
     */
    Point getCentroid() const override;

    /**
     * @brief This function doesn't really make sense for a plane but needs to be implemented since it's a pure virtual
     * function of Object. Does nothing.
     * @param pmin The bounding box's current lower bound.
     * @param pmax The bounding box's current higher bound.
     */
    void compareBoundingBox(Point& pmin, Point& pmax) const override;

    Point point;   ///< A point on the plane.
    Vector normal; ///< The plane's normal.
};

/**
 * @struct Sphere
 * @brief A sphere.
 */
struct Sphere : Object {
    /**
     * @brief Constructor. Creates a sphere with a plain color.
     * @param color The sphere's color.
     * @param center The sphere's center.
     * @param radius The sphere's radius.
     */
    Sphere(const Color& color, const Point& center, float radius);

    /**
     * @brief Constructor. Creates a sphere with a specific color function.
     * @param getColor The sphere's color function.
     * @param center The sphere's center.
     * @param radius The sphere's radius.
     */
    Sphere(const ColorFunc& getColor, const Point& center, float radius);

    /**
     * @return The object's type.
     */
    ObjectType getType() const override;

    /**
     * @brief Calculates the intersection between a ray and the sphere.
     * @param ray The ray to calculate the intersection with.
     * @return The information on the hit object. If no object is hit, the intersection will be set to infinity.
     */
    Hit intersect(const Ray& ray) const override;

    /**
     * @brief Calculates the centroid (barycenter) of the sphere.
     * @return The center of the sphere.
     */
    Point getCentroid() const override;

    /**
     * @brief Compares this sphere to a bounding box. Replaces the values of pmin and pmax's components if they are
     * respectively lower or higher.
     * @param pmin The bounding box's current lower bound.
     * @param pmax The bounding box's current higher bound.
     */
    void compareBoundingBox(Point& pmin, Point& pmax) const override;

    Point center; ///< The sphere's center.
    float radius; ///< The sphere's radius.
};

/**
 * @struct Triangle
 * @brief A triangle.
 */
struct Triangle : Object {
    /**
     * @brief Constructor. Creates a triangle with a plain color.
     * @param color The triangle's color.
     * @param A The triangle's first point.
     * @param B The triangle's second point.
     * @param C The triangle's third point.
     */
    Triangle(const Color& color, const Point& A, const Point& B, const Point& C);

    /**
     * @brief Constructor. Creates a triangle with a specific color function.
     * @param getColor The triangle's color function.
     * @param A The triangle's first point.
     * @param B The triangle's second point.
     * @param C The triangle's third point.
     */
    Triangle(const ColorFunc& getColor, const Point& A, const Point& B, const Point& C);

    /**
     * @return The object's type.
     */
    ObjectType getType() const override;

    /**
     * @brief Calculates the intersection between a ray and the triangle.
     * @param ray The ray to calculate the intersection with.
     * @return The information on the hit object. If no object is hit, the intersection will be set to infinity.
     */
    Hit intersect(const Ray& ray) const override;

    /**
     * @brief Calculates the centroid (barycenter) of the triangle.
     * @return The average of all three of the triangle's points.
     */
    Point getCentroid() const override;

    /**
     * @brief Compares this triangle to a bounding box. Replaces the values of pmin and pmax's components if they are
     * respectively lower or higher.
     * @param pmin The bounding box's current lower bound.
     * @param pmax The bounding box's current higher bound.
     */
    void compareBoundingBox(Point& pmin, Point& pmax) const override;

    Point A; ///< The triangle's first point.
    Point B; ///< The triangle's second point.
    Point C; ///< The triangle's third point.
};

/**
 * @struct MeshTriangle
 * @brief A triangle with multiple vertex attributes (positions and normals).
 */
struct MeshTriangle : Object {
    /**
     * @brief Constructor. Creates a mesh triangle with a plain color.
     * @param color The triangle's color.
     * @param A The triangle's first vertex.
     * @param B The triangle's second vertex.
     * @param C The triangle's third vertex.
     */
    MeshTriangle(const Color& color, const Vertex& A, const Vertex& B, const Vertex& C);
    MeshTriangle(const ColorFunc& getColor, const Vertex& A, const Vertex& B, const Vertex& C);

    /**
     * @return The object's type.
     */
    ObjectType getType() const override;

    /**
     * @brief Calculates the intersection between a ray and the triangle.
     * @param ray The ray to calculate the intersection with.
     * @return The information on the hit object. If no object is hit, the intersection will be set to infinity.
     */
    Hit intersect(const Ray& ray) const override;

    /**
     * @brief Calculates the centroid (barycenter) of the triangle.
     * @return The average of all three of the triangle's points.
     */
    Point getCentroid() const override;

    /**
     * @brief Compares this triangle to a bounding box. Replaces the values of pmin and pmax's components if they are
     * respectively lower or higher.
     * @param pmin The bounding box's current lower bound.
     * @param pmax The bounding box's current higher bound.
     */
    void compareBoundingBox(Point& pmin, Point& pmax) const override;

    Vertex A; ///< The triangle's first vertex.
    Vertex B; ///< The triangle's second vertex.
    Vertex C; ///< The triangle's third vertex.
};
