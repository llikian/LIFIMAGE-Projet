/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

#include <chrono>
#include "image.h"
#include "vec.h"

/**
 * @class Application
 * @brief
 */
class Application {
public:
    Application();
    ~Application();

    Image run(unsigned int width, unsigned int height);

private:
    void process(Image& image) const;
    Color processPixel(Point extremity) const;

    static constexpr vec2 offsets[4]{
        vec2(0.5f, 0.75f),
        vec2(-0.5f, -0.75f),
        vec2(-0.75f, 0.5f),
        vec2(0.75f, -0.5f)
    };

    Point camera;
};
