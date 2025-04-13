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
 * @brief Used to store information about a 3D scene and render it.
 */
class Scene {
public:
    /**
     * @brief Constructor. Initializes the scene.
     * @param name The scene's name.
     */
    explicit Scene(const std::string& name);

    /**
     * @brief Destructor. Frees all the lights and objects.
     */
    ~Scene();

    /**
     * @brief Renders the scene to an image. The image will be stored in "data/synthese/<scene_name>.png".
     * @param width The image's width.
     * @param height The image's height.
     */
    void render(unsigned int width, unsigned int height);

    /**
     * @brief Add a light to the scene.
     * @param light The light.
     */
    void add(const Light* light);

    /**
     * @brief Add an object to the scene.
     * @param object The object.
     */
    void add(const Object* object);

    /**
     * @brief Add a plane to the scene.
     * @param plane The plane.
     */
    void add(const Plane* plane);

    /**
     * @brief Add a mesh to the scene.
     * @param meshPath The path to the mesh.
     * @param transform The transform applied to every vertex.
     * @param getColor The mesh's color function.
     * @param smooth Whether to use smooth lighting.
     */
    void add(const std::string& meshPath, const mat4& transform, const ColorFunc& getColor, bool smooth = false);

    /**
     * @brief Add a mesh to the scene.
     * @param meshPath The path to the mesh.
     * @param transform The transform applied to every vertex.
     * @param color The mesh's color.
     * @param smooth Whether to use smooth lighting.
     */
    void add(const std::string& meshPath, const mat4& transform, const Color& color = White(), bool smooth = false);

    /**
     * @brief Add a mesh to the scene.
     * @param data The mesh's data.
     * @param transform The transform applied to every vertex.
     * @param getColor The mesh's color function.
     * @param smooth Whether to use smooth lighting.
     */
    void add(const MeshIOData& data, const mat4& transform, const ColorFunc& getColor, bool smooth = false);

    /**
     * @brief Add a mesh to the scene.
     * @param data The mesh's data.
     * @param transform The transform applied to every vertex.
     * @param color The mesh's color.
     * @param smooth Whether to use smooth lighting.
     */
    void add(const MeshIOData& data, const mat4& transform, const Color& color = White(), bool smooth = false);

    /**
     * @brief Add a mesh to the scene.
     * @param positions The mesh's positions.
     * @param transform The transform applied to every vertex.
     * @param getColor The mesh's color function.
     */
    void add(const std::vector<Point>& positions, const mat4& transform, const ColorFunc& getColor);

    /**
     * @brief Add a mesh to the scene.
     * @param positions The mesh's positions.
     * @param transform The transform applied to every vertex.
     * @param color The mesh's color.
     */
    void add(const std::vector<Point>& positions, const mat4& transform, const Color& color = White());

    /**
     * @brief Add a mesh to the scene.
     * @param positions The mesh's positions.
     * @param indices The mesh's position indices.
     * @param transform The transform applied to every vertex.
     * @param getColor The mesh's color function.
     */
    void add(const std::vector<Point>& positions,
             const std::vector<uint>& indices,
             const mat4& transform,
             const ColorFunc& getColor);

    /**
     * @brief Add a mesh to the scene.
     * @param positions The mesh's positions.
     * @param indices The mesh's position indices.
     * @param transform The transform applied to every vertex.
     * @param color The mesh's color.
     */
    void add(const std::vector<Point>& positions,
             const std::vector<uint>& indices,
             const mat4& transform,
             const Color& color = White());

    /**
     * @brief Calculates the intersection between a ray and the objects in the scene.
     * @param ray The ray to check the intersection with.
     * @return The information on the hit object. If no object is hit, the intersection will be set to infinity.
     */
    Hit getClosestHit(const Ray& ray) const;

    /**
     * @brief Changes the color the sky is at its lowest point.
     * @param r The red channel.
     * @param g The green channel.
     * @param b The blue channel.
     */
    void setLowSkyColor(float r, float g, float b);

    /**
     * @brief Changes the color the sky is at its highest point.
     * @param r The red channel.
     * @param g The green channel.
     * @param b The blue channel.
     */
    void setHighSkyColor(float r, float g, float b);

private:
    /**
     * @brief Computes an image.
     * @param image The image to save to.
     */
    void computeImage(Image& image);

    /**
     * @brief Computes a pixel's color.
     * @param extremity The point the ray is cast to.
     * @return The pixel's computed color.
     */
    Color computePixel(Point extremity) const;

    /**
     * @brief Prints info on the scene: The amount and type of lights and objects.
     */
    void printSceneInfo() const;

    std::string name; ///< The scene's name.

    unsigned int globalRow; ///< The current row being rendered.

    Point camera; ///< The camera's position.

    std::vector<const Light*> lights;   ///< The lights lighting up the scene.
    std::vector<const Object*> objects; ///< The objects inside the scene.
    std::vector<const Plane*> planes;   ///< The planes inside the scene.

    BVH bvh; ///< The bounding volume hierarchy used to calculate intersections.

    Color lowSkyColor;  ///< The color the sky is at its lowest point.
    Color highSkyColor; ///< The color the sky is at its highest point.
};
