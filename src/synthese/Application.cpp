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
    // lights.emplace_back(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-4.0f, 6.0f, 1.0f)));
    lights.emplace_back(new PointLight(Color(1.0f, 0.0f, 0.0f), Point(0.0f, 3.0f, -3.0f), 15.0f));
    lights.emplace_back(new PointLight(Color(0.0f, 1.0f, 0.0f), Point(-3.0f, 0.01f, -3.0f), 15.0f));
    lights.emplace_back(new PointLight(Color(0.0f, 0.0f, 1.0f), Point(3.0f, 0.01f, -3.0f), 15.0f));

    /* ---- Objects ---- */
    objects.push_back(new Plane(Color(0.267f, 0.749f, 0.267f), Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));
    objects.push_back(new Sphere(Color(1.0f, 0.0f, 0.0f), Point(0.0f, 0.0f, -3.0f), 1.0f));
}

Application::~Application() {
    for(const Object* object : objects) { delete object; }
    for(const Light* light : lights) { delete light; }
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

Color lerp(const Color& A, const Color& B, float t) {
    return A * (1.0f - t) + B * t;
}

Color Application::processPixel(Point extremity) const {
    static const Vector horizon(0.0f, 1.0f, 0.0f);

    const Ray ray(camera, normalize(Vector(camera, extremity)));

    Hit hit = getClosestHit(ray);
    if(hit.object == nullptr) {
        static const Color darkSky(0.239f, 0.29f, 0.761f);
        static const Color lightSky(0.671f, 0.851f, 1.0f);

        return lerp(lightSky, darkSky, (1.0f + dot(ray.direction, horizon)) / 2.0f);
    }

    Color color = static_cast<const Object*>(hit.object)->color;
    Point epsilonPoint = ray.getEpsilonPoint(hit);

    Ray lightRay(epsilonPoint, Vector());
    Color lightColor;
    float shadows = 1.0f;

    for(const Light* light : lights) {
        // Lighting
        Color lighting = light->calculate(hit, lightRay);

        // Shadows
        Hit closest = getClosestHit(lightRay);
        if(closest.object != nullptr && closest.object != hit.object) {
            lightColor += 0.2f * lighting;
        } else {
            lightColor += lighting;
        }
    }

    return color * lightColor * shadows;
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
