/***************************************************************************************************
 * @file  Scene.hpp
 * @brief Declaration of the Scene class
 **************************************************************************************************/

#pragma once

#include <string>
#include <vector>

#include "BVH.hpp"
#include "Hit.hpp"
#include "image.h"
#include "Light.hpp"
#include "mat4.hpp"
#include "mesh_io.h"
#include "Object.hpp"
#include "Ray.hpp"
#include "vec.h"

/**
 * @class Scene
 * @brief
 */
class Scene {
public:
    explicit Scene(const std::string& name);
    ~Scene();

    void render(unsigned int width, unsigned int height);

    void add(const Light* light);
    void add(const Object* object);
    void add(const Plane* plane);
    void add(const std::string& meshPath, const mat4& transform, const ColorFunc& getColor, bool smooth = false);
    void add(const std::string& meshPath, const mat4& transform, const Color& color = White(), bool smooth = false);
    void add(const MeshIOData& data, const mat4& transform, const ColorFunc& getColor, bool smooth = false);
    void add(const MeshIOData& data, const mat4& transform, const Color& color = White(), bool smooth = false);
    void add(const std::vector<Point>& positions, const mat4& transform, const ColorFunc& getColor);
    void add(const std::vector<Point>& positions, const mat4& transform, const Color& color = White());

    Hit getClosestHit(const Ray& ray) const;

    void setLightSkyColor(float r, float g, float b);
    void setDarkSkyColor(float r, float g, float b);

private:
    void computeImage(Image& image);
    Color computePixel(Point extremity) const;

    void printSceneInfo() const;

    std::string name;

    unsigned int globalRow;

    Point camera;
    std::vector<const Light*> lights;
    std::vector<const Object*> objects;
    std::vector<const Plane*> planes;

    BVH bvh;

    Color lightSky;
    Color darkSky;
};
