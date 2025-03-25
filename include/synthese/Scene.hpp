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
#include "mat4.hpp"
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
    void add(const std::string& meshPath, const mat4& transform, const Color& color = White());
    void add(const std::vector<Point>& meshData, const mat4& transform, const Color& color = White());

    const std::vector<const Light*>& getLights() const;
    const std::vector<const Object*>& getObjects() const;

    Hit getClosestHit(const Ray& ray) const;

private:
    void computeImage(Image& image);
    Color computePixel(Point extremity) const;

    Point camera;
    std::vector<const Light*> lights;
    std::vector<const Object*> objects;

    unsigned int globalRow;
};
