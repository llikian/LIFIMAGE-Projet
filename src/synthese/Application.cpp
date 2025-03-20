/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "synthese/Application.hpp"

#include <chrono>
#include <mutex>
#include <thread>
#include <vector>
#include "utility.hpp"

Application::Application() : camera(0.0f, 0.0f, 0.0f) { }

Application::~Application() { }

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

    return std::move(image);
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
    const float aspectRatio = columns / rows;

    mutex.lock();
    unsigned int row = globalRow++;
    mutex.unlock();

    Point extremity( 0.0f, 0.0f, -1.0f);
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
    return Color((extremity.x + 1.0f) / 2.0f, 0.0f, (extremity.y + 1.0f) / 2.0f);
}
