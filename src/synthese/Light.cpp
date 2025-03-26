/***************************************************************************************************
* @file  Light.cpp
 * @brief Implementation of the Light struct
 **************************************************************************************************/

#include "synthese/Light.hpp"

#include <cmath>

Light::Light(const Color& color) : color(color) { }

bool Light::isInShadow(const Object* object, const Ray& ray, const std::vector<const Object*>& objects) {
    Hit closest = getClosestHit(ray, objects);
    return closest.object != nullptr && closest.object != object;
}

DirectionalLight::DirectionalLight(const Color& color, const Vector& direction)
    : Light(color), direction(normalize(direction)) { }

Color DirectionalLight::calculate(const Hit& hit, const Point& point, const std::vector<const Object*>& objects) const {
    Ray ray(point, direction);

    if(isInShadow(hit.object, ray, objects)) { return Black(); }

    float cos_theta = std::max(dot(hit.normal, ray.direction), 0.0f);

    return color * cos_theta;
}

PointLight::PointLight(const Color& color, const Point& position, float radius)
    : Light(color), position(position), radius(radius) { }

Color PointLight::calculate(const Hit& hit, const Point& point, const std::vector<const Object*>& objects) const {
    Ray ray(point, position - point);
    float distance2 = length(ray.direction);
    ray.direction = ray.direction / std::sqrt(distance2);

    if(isInShadow(hit.object, ray, objects)) { return Black(); }

    float attenuation = std::clamp(radius * radius / (1.0f + distance2), 0.0f, 1.0f);
    float cos_theta = std::max(dot(hit.normal, ray.direction), 0.0f);

    return color * attenuation * cos_theta;
}
