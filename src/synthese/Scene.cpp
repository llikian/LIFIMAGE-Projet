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
    : name(name),
      globalRow(0),
      bvh(objects),
      lowSkyColor(0.671f, 0.851f, 1.0f), highSkyColor(0.239f, 0.29f, 0.761f) { }

Scene::~Scene() {
    for(const Light* light : lights) { delete light; }
    for(const Object* object : objects) { delete object; }
    for(const Plane* plane : planes) {delete plane;}
}

void Scene::render(unsigned int width, unsigned int height) {
    if(width == 0 || height == 0) { throw std::runtime_error("Cannot render to an empty image."); }

    std::cout << "Rendering scene \"" << name << "\" to a " << width << " by " << height << " image.\n";
    printSceneInfo();

    bvh.initialize();

    Image image(width, height);
    std::vector<std::thread> threads;
    const std::chrono::time_point startTime(std::chrono::high_resolution_clock::now());

    unsigned int threadCount = std::thread::hardware_concurrency();
    globalRow = 0;

    std::cout << "\tDispatching " << threadCount << " threads...\n";
    for(unsigned int i = 0 ; i < threadCount ; ++i) {
        threads.emplace_back(&Scene::computeImage, this, std::ref(image));
    }

    for(std::thread& thread : threads) { thread.join(); }

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

void Scene::add(const Plane* plane) {
    planes.push_back(plane);
}

void Scene::add(const std::string& meshPath, const mat4& transform, const ColorFunc& getColor, bool smooth) {
    if(smooth) {
        MeshIOData data;
        read_meshio_data(meshPath.c_str(), data);
        add(data, transform, getColor, smooth);
    } else {
        std::vector<Point> positions;
        read_positions(meshPath.c_str(), positions);
        add(positions, transform, getColor);
    }
}

void Scene::add(const std::string& meshPath, const mat4& transform, const Color& color, bool smooth) {
    add(meshPath, transform, [color](const Point&) { return color; }, smooth);
}

void Scene::add(const MeshIOData& data, const mat4& transform, const ColorFunc& getColor, bool smooth) {
    if(smooth) {
        for(unsigned int i = 0 ; i + 2 < data.indices.size() ; i += 3) {
            unsigned int index0 = data.indices.at(i);
            unsigned int index1 = data.indices.at(i + 1);
            unsigned int index2 = data.indices.at(i + 2);

            add(new MeshTriangle(getColor,
                                 Vertex(transform * data.positions.at(index0), data.normals.at(index0)),
                                 Vertex(transform * data.positions.at(index1), data.normals.at(index1)),
                                 Vertex(transform * data.positions.at(index2), data.normals.at(index2))));
        }
    } else {
        add(data.positions, data.indices, transform, getColor);
    }
}

void Scene::add(const MeshIOData& data, const mat4& transform, const Color& color, bool smooth) {
    add(data, transform, [color](const Point&) { return color; }, smooth);
}

void Scene::add(const std::vector<Point>& positions, const mat4& transform, const ColorFunc& getColor) {
    for(unsigned int i = 0 ; i + 2 < positions.size() ; i += 3) {
        add(new Triangle(getColor,
                         transform * positions[i],
                         transform * positions[i + 1],
                         transform * positions[i + 2]));
    }
}

void Scene::add(const std::vector<Point>& positions, const mat4& transform, const Color& color) {
    add(positions, transform, [color](const Point&) { return color; });
}

void Scene::add(const std::vector<Point>& positions,
                const std::vector<uint>& indices,
                const mat4& transform,
                const ColorFunc& getColor) {
    for(unsigned int i = 0 ; i + 2 < indices.size() ; i += 3) {
        add(new Triangle(getColor,
                         transform * positions.at(indices.at(i)),
                         transform * positions.at(indices.at(i + 1)),
                         transform * positions.at(indices.at(i + 2))));
    }
}

void Scene::add(const std::vector<Point>& positions,
                const std::vector<uint>& indices,
                const mat4& transform,
                const Color& color) {
    add(positions, indices, transform, [color](const Point&) { return color; });
}

Hit Scene::getClosestHit(const Ray& ray) const {
    Hit closest = bvh.intersect(ray);

    for(const Plane* plane : planes) {
        Hit hit = plane->intersect(ray);

        if(hit.intersection != infinity && (closest.object == nullptr || hit.intersection < closest.intersection)) {
            closest.intersection = hit.intersection;
            closest.normal = hit.normal;
            closest.object = plane;
        }
    }

    return closest;
}

void Scene::setLowSkyColor(float r, float g, float b) {
    lowSkyColor.r = r;
    lowSkyColor.g = g;
    lowSkyColor.b = b;
}

void Scene::setHighSkyColor(float r, float g, float b) {
    highSkyColor.r = r;
    highSkyColor.g = g;
    highSkyColor.b = b;
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
    if(closest.object == nullptr) { return lerp(lowSkyColor, highSkyColor, (1.0f + dot(ray.direction, horizon)) / 2.0f); }

    Point point = ray.getPoint(closest.intersection);
    Point epsilonPoint = ray.getEpsilonPoint(closest);
    Color color = closest.object->getColor(point);

    Color lightColor;
    for(const Light* light : lights) { lightColor += light->calculate(closest, epsilonPoint, this); }

    return color * lightColor;
}

void Scene::printSceneInfo() const {
    static constexpr unsigned char lightTypeCount = static_cast<unsigned char>(LightType::TYPE_COUNT);
    static constexpr unsigned char objectTypeCount = static_cast<unsigned char>(ObjectType::TYPE_COUNT);

    int lightCounts[lightTypeCount];
    for(int& lightCount : lightCounts) { lightCount = 0; }
    for(const Light* light : lights) { lightCounts[static_cast<unsigned char>(light->getType())]++; }

    int objectCounts[objectTypeCount];
    for(int& objectCount : objectCounts) { objectCount = 0; }
    objectCounts[static_cast<unsigned char>(ObjectType::Plane)] += planes.size();
    for(const Object* object : objects) { objectCounts[static_cast<unsigned char>(object->getType())]++; }

    std::cout << "\tThere are:\n";
    for(unsigned int i = 0 ; i < lightTypeCount ; ++i) {
        if(lightCounts[i] > 0) {
            LightType type = static_cast<LightType>(i);

            std::cout << "\t\t" << lightCounts[i] << ' ';
            switch(type) {
                case LightType::DirectionalLight: std::cout << "Directional Light";
                    break;
                case LightType::PointLight: std::cout << "Point Light";
                    break;
                default: break;
            }

            std::cout << (lightCounts[i] > 1 ? "s" : "") << '\n';
        }
    }

    for(unsigned int i = 0 ; i < objectTypeCount ; ++i) {
        if(objectCounts[i] > 0) {
            ObjectType type = static_cast<ObjectType>(i);
            std::cout << "\t\t" << objectCounts[i] << ' ';

            switch(type) {
                case ObjectType::Plane: std::cout << "Plane";
                    break;
                case ObjectType::Sphere: std::cout << "Sphere";
                    break;
                case ObjectType::Triangle: std::cout << "Triangle";
                    break;
                case ObjectType::MeshTriangle: std::cout << "MeshTriangle";
                    break;
                default: break;
            }

            std::cout << (objectCounts[i] > 1 ? "s" : "") << '\n';
        }
    }
}
