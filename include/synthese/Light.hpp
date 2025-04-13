/***************************************************************************************************
 * @file  Light.hpp
 * @brief Declaration of the Light struct
 **************************************************************************************************/

#pragma once

#include "color.h"
#include "Ray.hpp"
#include "vec.h"

class Scene; // Forward-Declared to avoid circular dependency.

/**
 * @enum LightType
 * @brief Enumeration of all Light types.
 */
enum class LightType : unsigned char {
    DirectionalLight,
    PointLight,
    TYPE_COUNT
};

/**
 * @struct Light
 * @brief A light, used to light up a scene.
 */
struct Light {
    /**
     * @brief Constructor. Creates a Light with a specific color.
     * @param color
     */
    explicit Light(const Color& color);

    /**
     * @brief Destructor.
     */
    virtual ~Light() = default;

    /**
     * @return The type of Light.
     */
    virtual LightType getType() const = 0;

    /**
     * @brief Calculates the light for an intersection in a scene.
     * @param hit The intersection we calculate the light for.
     * @param point The position of the hit object.
     * @param scene The scene we calculate the light for.
     * @return The color of the light at this intersection.
     */
    virtual Color calculate(const Hit& hit, const Point& point, const Scene* scene) const = 0;

    /**
     * @brief Checks if a point is in the shadow cast by the light because of an object.
     * @param object The object the point belongs to.
     * @param ray The ray with its origin being the point we want to check and pointing towards the light.
     * @param scene The scene we calculate the light for.
     * @return
     */
    static bool isInShadow(const Object* object, const Ray& ray, const Scene* scene);

    Color color; ///< The light's color.
};

/**
 * @struct DirectionalLight
 * @brief A directional light, lights up the entire scene in a certain direction, akin to a sun.
 */
struct DirectionalLight : Light {
    /**
     * @brief Constructor. Creates a directional light with a certain color and lighting up a certain direction.
     * @param color The light's color.
     * @param direction The direction of the light. Goes "towards" the light and not "from" it.
     */
    DirectionalLight(const Color& color, const Vector& direction);

    /**
     * @return The type of Light.
     */
    LightType getType() const override;

    /**
     * @brief Calculates the light for an intersection in a scene.
     * @param hit The intersection we calculate the light for.
     * @param point The position of the hit object.
     * @param scene The scene we calculate the light for.
     * @return The color of the light at this intersection.
     */
    Color calculate(const Hit& hit, const Point& point, const Scene* scene) const override;

    Vector direction; ///< The direction of the light. Goes "towards" the light and not "from" it.
};

/**
 * @struct PointLight
 * @brief A point light, lights up the scene at a specific position and radius.
 */
struct PointLight : Light {
    /**
     * @brief Constructor. Creates a point light with a certain color and lighting up a certain part of the scene.
     * @param color The light's color.
     * @param position The light's position.
     * @param radius The light's radius.
     */
    PointLight(const Color& color, const Point& position, float radius);

    /**
     * @return The type of Light.
     */
    LightType getType() const override;

    /**
     * @brief Calculates the light for an intersection in a scene.
     * @param hit The intersection we calculate the light for.
     * @param point The position of the hit object.
     * @param scene The scene we calculate the light for.
     * @return The color of the light at this intersection.
     */
    Color calculate(const Hit& hit, const Point& point, const Scene* scene) const override;

    Point position; ///< The light's position.
    float radius;   ///< The light's radius.
};
