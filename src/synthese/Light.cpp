/***************************************************************************************************
* @file  Light.cpp
 * @brief Implementation of the Light struct
 **************************************************************************************************/

#include "synthese/Light.hpp"

#include <cmath>

#include "utility.hpp"
#include "synthese/Scene.hpp"

Light::Light(const Color& color) : color(color) { }

bool Light::isInShadow(const Object* object, const Ray& ray, const Scene* scene) {
    const Hit closest = scene->getClosestHit(ray);
    return closest.object != nullptr && closest.object != object;
}

DirectionalLight::DirectionalLight(const Color& color, const Vector& direction)
    : Light(color), direction(normalize(direction)) { }

LightType DirectionalLight::getType() const {
    return LightType::DirectionalLight;
}

Color DirectionalLight::calculate(const Hit& hit, const Point& point, const Scene* scene) const {
    Ray ray(point, direction);

    if(isInShadow(hit.object, ray, scene)) { return Black(); }

    float cos_theta = std::max(dot(hit.normal, ray.direction), 0.0f);

    return color * cos_theta;
}

PointLight::PointLight(const Color& color, const Point& position, float radius)
    : Light(color), position(position), radius(radius) { }

LightType PointLight::getType() const {
    return LightType::PointLight;
}

Color PointLight::calculate(const Hit& hit, const Point& point, const Scene* scene) const {
    Vector direction(point, position);
    float distance = length(direction);
    Ray ray(point, direction * (1.0f / distance));

    if(isInShadow(hit.object, ray, scene)) { return Black(); }

    float windowing = pow2(std::max(1.0f - pow2(distance / radius), 0.0f));
    float attenuation = windowing * (pow2(radius) / (pow2(distance) + radius));
    float cos_theta = std::max(dot(hit.normal, ray.direction), 0.0f);

    return color * (attenuation * cos_theta);
}
