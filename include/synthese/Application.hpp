/***************************************************************************************************
 * @file  Application.hpp
 * @brief Declaration of the Application class
 **************************************************************************************************/

#pragma once

#include <vector>
#include "image.h"
#include "Light.hpp"
#include "Object.hpp"
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
    Hit getClosestHit(const Ray& ray) const;

    Point camera;

    std::vector<const Object*> objects;
    std::vector<const Light*> lights;
};
