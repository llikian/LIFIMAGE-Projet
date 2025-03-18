/***************************************************************************************************
 * @file  Application.cpp
 * @brief Implementation of the Application class
 **************************************************************************************************/

#include "Application.hpp"

#include <mutex>
#include <thread>
#include <vector>
#include "utility.hpp"

Application::Application()
    : camera(0.0f, 0.0f, 0.0f) { }

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

    return image;
}

static unsigned int getRows(unsigned int rows) {
    static unsigned int row = 0;
    static std::mutex mutex;

    std::unique_lock lock(mutex);

    if(row < rows) {
        return row++;
    } else {
        return -1;
    }
}

void Application::process(Image& image) const {
    unsigned int rows = image.height();
    unsigned int columns = image.width();

    static const vec2 offsets[4]{
        vec2(0.5f, 0.75f),
        vec2(-0.5f, -0.75f),
        vec2(-0.75f, 0.5f),
        vec2(0.75f, -0.5f)
    };

    unsigned int row = getRows(rows);
    while(row < rows) {
        for(unsigned int column = 0 ; column < columns ; ++column) {
            Color color;

            for(const vec2& offset : offsets) {
                color += processPixel(Point((2.0f * column - (columns + offset.x)) / rows,
                                            (2.0f * row - (rows + offset.y)) / rows,
                                            -1.0f));
            }

            Color& pixel = image(column, row);
            pixel = color / 4.0f;
            pixel.a = 1.0f;
        }

        row = getRows(rows);
    }
}

Color Application::processPixel(Point extremity) const {
    return Color((extremity.x + 1.0f) / 2.0f, 0.0f, (extremity.y + 1.0f) / 2.0f);
}
