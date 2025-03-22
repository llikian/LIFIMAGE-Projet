/***************************************************************************************************
 * @file  Scene.hpp
 * @brief Declaration of the Scene class
 **************************************************************************************************/

#pragma once

#include <string>
#include <vector>
#include "Hit.hpp"
#include "image.h"
#include "Light.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "vec.h"

/**
 * @class Scene
 * @brief
 */
class Scene {
public:
    Scene();
    ~Scene();

    void render(const std::string& path, unsigned int width, unsigned int height);

    void add(const Light* light);
    void add(const Object* object);
    void add(const std::string& meshPath, const Vector& translation, const Color& color = White());
    void add(const std::vector<Point>& meshData, const Vector& translation, const Color& color = White());

private:
    void computeImage(Image& image);
    Color computePixel(Point extremity) const;
    Hit getClosestHit(const Ray& ray) const;

    Point camera;
    std::vector<const Light*> lights;
    std::vector<const Object*> objects;

    unsigned int globalRow;
};
