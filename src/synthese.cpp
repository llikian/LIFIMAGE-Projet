/***************************************************************************************************
 * @file  synthese.cpp
 * @brief Contains the main program for the 'synthese' executable
 **************************************************************************************************/

#include "synthese/Scene.hpp"

#include <iostream>

Scene scene1() {
    Scene scene;

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-4.0f, 6.0f, 1.0f)));
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(4.0f, 6.0f, 1.0f)));

    /* ---- Objects ---- */
    scene.add(new Plane([](const Point& point) {
        return std::fmod(floor(point.x) + floor(point.z), 2.0f) == 0.0f ? White() : Black(); // Checker
    }, Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    {
        Point center(0.0f, 0.0f, -3.0f);
        scene.add(new Sphere([center](const Point& point) {
            Vector v = (normalize(point - center) + Vector(1.0f, 1.0f, 1.0f)) / 2.0f;
            return Color(v.x, v.y, v.z);
        }, center, 1.0f));
    }

    return scene;
}

Scene scene2() {
    Scene scene;

    /* ---- Lights ---- */
    {
        float radius = 5.0f;
        // scene.add(new PointLight(Red(), Point(0.0f, 0.01f, -3.0f), radius));
        scene.add(new PointLight(Green(), Point(2.0f, 0.01f, -3.0f), radius));
        scene.add(new PointLight(Blue(), Point(-2.0f, 0.01f, -3.0f), radius));
    }

    /* ---- Objects ---- */
    scene.add(new Plane([](const Point& point) {
        return std::fmod(floor(point.x) + floor(point.z), 2.0f) == 0.0f ? White() : Black(); // Checker
    }, Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    scene.add(new Sphere(Color(0.82f, 0.2f, 0.2f), Point(-1.0f, 1.0f, -3.0f), 1.0f));
    scene.add(new Sphere(Color(0.2f, 0.2f, 0.82f), Point(1.0f, 1.0f, -3.0f), 1.0f));

    return scene;
}

Scene scene3() {
    Scene scene;

    /* ---- Lights ---- */


    /* ---- Objects ---- */


    return scene;
}

int main() {
    try {
        // scene1().render("data/synthese/scene1.png", 1024, 512);
        // scene2().render("data/synthese/scene2.png", 1024, 512);
        scene3().render("data/synthese/scene3.png", 1024, 512);
    } catch(const std::exception& exception) {
        std::cerr << "ERROR : " << exception.what() << '\n';
        return -1;
    }

    return 0;
}
