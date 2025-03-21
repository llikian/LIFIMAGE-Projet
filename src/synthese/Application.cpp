/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "synthese/Application.hpp"

#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include <synthese/Ray.hpp>

#include "utility.hpp"

Application::Application()
    : camera(0.0f, 0.0f, 0.0f) {
    /* ---- Lights ---- */
    lights.emplace_back(normalize(Vector(-4.0f, 6.0f, 1.0f)), Color(1.0f, 1.0f, 1.0f));
    // lights.emplace_back(normalize(Vector(4.0f, 6.0f, -1.0f)), Color(1.0f, 1.0f, 1.0f));

    /* ---- Objects ---- */
    objects.push_back(new Plane(Color(0.267f, 0.749f, 0.267f), Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));
    objects.push_back(new Sphere(Color(1.0f, 0.0f, 0.0f), Point(0.0f, 0.0f, -3.0f), 1.0f));
}

Application::~Application() {
    for(const Object* object : objects) {
        delete object;
    }
}

Image Application::run(unsigned int width, unsigned int height) {
    const std::chrono::time_point startTime(std::chrono::high_resolution_clock::now());

    Image image(width, height);

    std::vector<std::thread> threads;
    unsigned int threadCount = std::thread::hardware_concurrency();

    std::cout << "Dispatching " << threadCount << " threads...\n";
    for(unsigned int i = 0 ; i < threadCount ; ++i) {
        threads.emplace_back(&Application::process, this, std::ref(image));
    }

    for(std::thread& thread : threads) {
        thread.join();
    }

    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - startTime;
    std::cout << "The image took " << duration.count() << "s to compute.\n";

    return image;
}

void Application::process(Image& image) const {
    static unsigned int globalRow = 0;
    static std::mutex mutex;

    static const vec2 offsets[4]{
        vec2(0.5f, 0.75f),
        vec2(-0.5f, -0.75f),
        vec2(-0.75f, 0.5f),
        vec2(0.75f, -0.5f)
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

                color += processPixel(extremity);
            }

            Color& pixel = image(column, row);
            pixel = color / 4.0f;
            pixel.a = 1.0f;
        }

        mutex.lock();
        row = globalRow++;
        mutex.unlock();
    }
}

Color Application::processPixel(Point extremity) const {
    static const Color background(0.447f, 0.643f, 0.89f);
    static const Vector horizon(0.0f, 1.0f, 0.0f);

    const Ray ray(camera, normalize(Vector(camera, extremity)));

    Hit hit = getClosestHit(ray);
    if(hit.object == nullptr) {
        return background;
    }

    Color color = static_cast<const Object*>(hit.object)->color;
    Point epsilonPoint = ray.getEpsilonPoint(hit);

    Color lightColor;
    float shadows = 1.0f;

    for(const Light& light : lights) {
        // Lighting
        lightColor += light.color * std::max(dot(hit.normal, light.direction), 0.0f);

        // Shadows
        Hit closest = getClosestHit(Ray(epsilonPoint, light.direction));
        if(closest.object != nullptr && closest.object != hit.object) {
            shadows *= 0.2f;
        }
    }

    return color * lightColor / lights.size() * shadows;
}

Hit Application::getClosestHit(const Ray& ray) const {
    Hit closest;

    for(const Object* object : objects) {
        Hit hit = object->intersect(ray);

        if(hit.intersection != infinity) {
            if(closest.object == nullptr || hit.intersection < closest.intersection) {
                closest.intersection = hit.intersection;
                closest.normal = hit.normal;
                closest.object = object;
            }
        }
    }

    return closest;
}
