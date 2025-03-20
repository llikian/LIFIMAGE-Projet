/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

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

    Point camera;
};
