/***************************************************************************************************
 * @file  Scene.cpp
 * @brief Implementation of the Scene class
 **************************************************************************************************/

#include "synthese/Scene.hpp"

#include <chrono>
#include <mutex>
#include <thread>
#include "image_io.h"
#include "mesh_io.h"
#include "utility.hpp"

Scene::Scene(const std::string& name)
    : globalRow(0), lightSky(0.671f, 0.851f, 1.0f), darkSky(0.239f, 0.29f, 0.761f), name(name) { }

Scene::~Scene() {
    for(const Object* object : objects) { delete object; }
    for(const Light* light : lights) { delete light; }
}

void Scene::render(unsigned int width, unsigned int height) {
    if(objects.empty()) { throw std::runtime_error("Cannot render an empty scene."); }
    if(width == 0 || height == 0) { throw std::runtime_error("Cannot render to an empty image."); }

    std::cout << "Rendering scene \"" << name << "\" to a " << width << " by " << height << " image.\n";

    Image image(width, height);
    std::vector<std::thread> threads;
    const std::chrono::time_point startTime(std::chrono::high_resolution_clock::now());

    unsigned int threadCount = std::thread::hardware_concurrency();
    globalRow = 0;

    std::cout << "\tDispatching " << threadCount << " threads...\n";
    for(unsigned int i = 0 ; i < threadCount ; ++i) {
        threads.emplace_back(&Scene::computeImage, this, std::ref(image));
    }

    for(std::thread& thread : threads) {
        thread.join();
    }

    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
    std::cout << "The image took " << duration.count() << "s to compute.\n\n";

    write_image(image, ("data/synthese/" + name + ".png").c_str());
}

void Scene::add(const Light* light) {
    lights.push_back(light);
}

void Scene::add(const Object* object) {
    objects.push_back(object);
}

void Scene::add(const std::string& meshPath, const mat4& transform, const Color& color, bool smooth) {
    MeshIOData data;
    read_meshio_data(meshPath.c_str(), data);
    add(data, transform, color, smooth);
}

void Scene::add(const MeshIOData& data, const mat4& transform, const Color& color, bool smooth) {
    for(unsigned int i = 0 ; i + 2 < data.indices.size() ; i += 3) {
        unsigned int index0 = data.indices.at(i);
        unsigned int index1 = data.indices.at(i + 1);
        unsigned int index2 = data.indices.at(i + 2);

        if(smooth) {
            add(new MeshTriangle(color,
                                 Vertex(transform * data.positions.at(index0), data.normals.at(index0)),
                                 Vertex(transform * data.positions.at(index1), data.normals.at(index1)),
                                 Vertex(transform * data.positions.at(index2), data.normals.at(index2))));
        } else {
            add(new Triangle(color,
                             transform * data.positions.at(index0),
                             transform * data.positions.at(index1),
                             transform * data.positions.at(index2)));
        }
    }
}

void Scene::add(const std::vector<Point>& positions, const mat4& transform, const Color& color) {
    for(unsigned int i = 0 ; i + 2 < positions.size() ; i += 3) {
        add(new Triangle(color,
                         transform * positions[i],
                         transform * positions[i + 1],
                         transform * positions[i + 2]));
    }
}

Hit Scene::getClosestHit(const Ray& ray) const {
    Hit closest;

    for(const Object* object : objects) {
        Hit hit = object->intersect(ray);

        if(hit.intersection != infinity && (closest.object == nullptr || hit.intersection < closest.intersection)) {
            closest.intersection = hit.intersection;
            closest.normal = hit.normal;
            closest.object = object;
        }
    }

    return closest;
}

void Scene::setLightSkyColor(float r, float g, float b) {
    lightSky.r = r;
    lightSky.g = g;
    lightSky.b = b;
}

void Scene::setDarkSkyColor(float r, float g, float b) {
    darkSky.r = r;
    darkSky.g = g;
    darkSky.b = b;
}

void Scene::computeImage(Image& image) {
    static std::mutex mutex;

    static const vec2 offsets[4]{
        vec2(0.125f, 0.375f),
        vec2(-0.125f, -0.375f),
        vec2(-0.375f, 0.125f),
        vec2(0.375f, -0.125f)
    };

    const unsigned int rows = image.height();
    const unsigned int columns = image.width();

    mutex.lock();
    unsigned int row = globalRow++;
    mutex.unlock();

    Point extremity(0.0f, 0.0f, -1.0f);
    while(row < rows) {
        for(unsigned int column = 0 ; column < columns ; ++column) {
            Color color;

            for(const vec2& offset : offsets) {
                extremity.x = (2.0f * (column + offset.x) - columns) / rows;
                extremity.y = (2.0f * (row + offset.y) - rows) / rows;

                color += computePixel(extremity);
            }

            Color& pixel = image(column, row);
            pixel = 0.25f * color;
            pixel.a = 1.0f;
        }

        mutex.lock();
        row = globalRow++;
        mutex.unlock();
    }
}

Color Scene::computePixel(Point extremity) const {
    static const Vector horizon(0.0f, 1.0f, 0.0f);

    const Ray ray(camera, normalize(Vector(camera, extremity)));

    Hit closest = getClosestHit(ray);
    if(closest.object == nullptr) { return lerp(lightSky, darkSky, (1.0f + dot(ray.direction, horizon)) / 2.0f); }

    Point point = ray.getPoint(closest.intersection);
    Point epsilonPoint = ray.getEpsilonPoint(closest);
    Color color = closest.object->getColor(point);

    Color lightColor;
    for(const Light* light : lights) { lightColor += light->calculate(closest, epsilonPoint, objects); }

    return color * lightColor;
}
