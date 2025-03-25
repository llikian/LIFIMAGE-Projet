/***************************************************************************************************
 * @file  synthese.cpp
 * @brief Contains the main program for the 'synthese' executable
 **************************************************************************************************/

#include "synthese/Scene.hpp"

#include <iostream>
#include <synthese/transforms.hpp>

#include "mesh_io.h"
#include "utility.hpp"

Scene scene1() {
    Scene scene;

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-4.0f, 6.0f, 1.0f)));
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(4.0f, 6.0f, 1.0f)));

    /* ---- Objects ---- */
    scene.add(new Plane([](const Point& point) {
        return std::fmod(floor(point.x) + floor(point.z), 2.0f) == 0.0f ? White() : Black(); // Checker
    }, Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f))); {
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
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-4.0f, 6.0f, 1.0f)));

    /* ---- Objects ---- */
    scene.add(new Plane([](const Point& point) {
        return std::fmod(floor(point.x) + floor(point.z), 2.0f) == 0.0f ? White() : Black(); // Checker
    }, Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f))); {
        std::vector<Point> positions;
        read_positions("data/synthese/cube.obj", positions);

        scene.add(positions, translate(-2.0f, 0.0f, -3.0f), Red());
        scene.add(positions, translate(2.0f, 0.0f, -3.0f), Blue());
        scene.add(positions, translate(0.0f, 2.0f, -3.0f), Green());
    }

    return scene;
}

Scene scene4() {
    Scene scene;

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-4.0f, 6.0f, 4.0f)));

    /* ---- Objects ---- */
    scene.add(new Plane([](const Point& point) {
        float t = std::fmod(std::floor(point.x) + std::floor(point.z), 2.0f);
        return t == 0.0f ? Color(0.922f, 0.216f, 0.216f) : White(); // Checker
    }, Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    /* Sphere 1 */ {
        Point pos = Point(0.0f, 0.0f, -2.5f);
        scene.add(new Sphere([pos](const Point& point) {
            Vector v = point - pos;
            float t = 0.5f + 0.5f * std::cos(15.0f * M_PIf * std::sqrt(std::abs(2.0f * v.y * v.x)));
            return lerp(Color(0.216f, 0.51f, 0.922f), Blue(), t);
        }, pos, 1.0f));
    }

    /* Sphere 2 */ {
        Point pos = Point(-2.5f, 0.0f, -2.5f);
        scene.add(new Sphere([pos](const Point& point) {
            Vector v = point - pos;
            float t = 0.5f + 0.5f * std::cos(15.0f * M_PIf * std::sqrt(std::abs(2.0f * v.y * v.x)));
            return lerp(Color(0.922f, 0.216f, 0.51f), Red(), t);
        }, pos, 1.0f));
    }

    /* Sphere 3 */ {
        Point pos = Point(2.5f, 0.0f, -2.5f);
        scene.add(new Sphere([pos](const Point& point) {
            Vector v = point - pos;
            float t = 0.5f + 0.5f * std::cos(15.0f * M_PIf * std::sqrt(std::abs(2.0f * v.y * v.x)));
            return lerp(Color(0.216f, 0.922f, 0.51f), Green(), t);
        }, pos, 1.0f));
    }

    return scene;
}

Scene scene5() {
    Scene scene;

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-4.0f, 6.0f, 1.0f)));
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(4.0f, 6.0f, 1.0f)));

    /* ---- Objects ---- */
    scene.add("data/synthese/dodecahedron.obj", translate(-2.0f, 0.0f, -4.0f).scale(2.0f), White());
    scene.add("data/synthese/cube.obj", translate(2.0f, 0.0f, -4.0f).scale(0.5f), Red());

    return scene;
}

Scene scene6() {
    Scene scene;

    /* ---- Sky ---- */
    scene.setLightSkyColor(0.3f, 0.3f, 0.3f);
    scene.setDarkSkyColor(0.1f, 0.1f, 0.1f);

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-4.0f, 6.0f, 10.0f)));

    /* ---- Objects ---- */
    MeshIOData suzanne;
    read_meshio_data("data/synthese/suzanne.obj", suzanne);
    scene.add(suzanne, translate(-1.0f, 0.2f, -2.0f).rotateY(-10.0f), White(), true);
    scene.add(suzanne, translate(1.0f, -0.2f, -2.0f).rotateY(10.0f).rotateZ(180.0f), White(), false);

    return scene;
}

int main() {
    try {
        // scene1().render("data/synthese/scene1.png", 1024, 512);
        // scene2().render("data/synthese/scene2.png", 1024, 512);
        // scene3().render("data/synthese/scene3.png", 1024, 512);
        // scene4().render("data/synthese/scene4.png", 1024, 512);
        // scene5().render("data/synthese/scene5.png", 1024, 512);
        scene6().render("data/synthese/scene6.png", 768, 512);
    } catch(const std::exception& exception) {
        std::cerr << "ERROR : " << exception.what() << '\n';
        return -1;
    }

    return 0;
}
