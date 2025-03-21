/***************************************************************************************************
* @file  Light.cpp
 * @brief Implementation of the Light struct
 **************************************************************************************************/

#include "synthese/Light.hpp"

#include <cmath>

Light::Light(const Color& color) : color(color) { }

DirectionalLight::DirectionalLight(const Color& color, const Vector& direction)
    : Light(color), direction(normalize(direction)) { }

Color DirectionalLight::calculate(const Hit& hit, Ray& ray) const {
    ray.direction = direction;
    return color * std::max(dot(hit.normal, ray.direction), 0.0f);
}

PointLight::PointLight(const Color& color, const Point& position, float linear, float quadratic)
    : Light(color), position(position), linear(linear), quadratic(quadratic) { }

Color PointLight::calculate(const Hit& hit, Ray& ray) const {
    ray.direction = position - ray.origin;
    float distance = length(ray.direction);
    ray.direction = ray.direction / distance;

    float attenuation = 1.0f / (1.0f + distance * (linear + distance * quadratic));

    return color * attenuation * std::max(dot(hit.normal, ray.direction), 0.0f);
}

SpotLight::SpotLight(const Color& color, const Point& position, const Vector& direction, float radius)
    : Light(color), position(position), direction(direction), radius(radius) { }

Color SpotLight::calculate(const Hit& hit, Ray& ray) const {
    return Color(); // TODO : Implement
}
