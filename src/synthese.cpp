/***************************************************************************************************
 * @file  synthese.cpp
 * @brief Contains the main program for the 'synthese' executable
 **************************************************************************************************/

#include "synthese/Scene.hpp"

#include <iostream>
#include <synthese/transforms.hpp>

#include "mesh_io.h"
#include "utility.hpp"

void scene1(unsigned int width, unsigned int height) {
    Scene scene("scene1");

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

    scene.render(width, height);
}

void scene2(unsigned int width, unsigned int height) {
    Scene scene("scene2");

    /* ---- Lights ---- */
    {
        float radius = 6.0f;
        scene.add(new PointLight(Blue(), Point(-2.0f, 1.0f, -1.0f), radius));
        scene.add(new PointLight(Red(), Point(2.0f, 1.0f, -1.0f), radius));
    }

    /* ---- Objects ---- */
    scene.add(new Plane([](const Point& point) {
        return std::fmod(floor(point.x) + floor(point.z), 2.0f) == 0.0f ? White() : Black(); // Checker
    }, Point(0.0f, -1.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f)));

    scene.add(new Sphere(Color(0.82f, 0.2f, 0.2f), Point(-1.0f, 1.0f, -3.0f), 1.0f));
    scene.add(new Sphere(Color(0.2f, 0.2f, 0.82f), Point(1.0f, 1.0f, -3.0f), 1.0f));

    scene.render(width, height);
}

void scene3(unsigned int width, unsigned int height) {
    Scene scene("scene3");

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

    scene.render(width, height);
}

void scene4(unsigned int width, unsigned int height) {
    Scene scene("scene4");

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

    scene.render(width, height);
}

void scene5(unsigned int width, unsigned int height) {
    Scene scene("scene5");

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-4.0f, 6.0f, 1.0f)));
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(4.0f, 6.0f, 1.0f)));

    /* ---- Objects ---- */
    scene.add("data/synthese/dodecahedron.obj", translate(-2.0f, 0.0f, -4.0f).scale(2.0f), White());
    scene.add("data/synthese/cube.obj", translate(2.0f, 0.0f, -4.0f).scale(0.5f), Red());

    scene.render(width, height);
}

void scene6(unsigned int width, unsigned int height) {
    Scene scene("scene6");

    /* ---- Sky ---- */
    scene.setLightSkyColor(0.3f, 0.3f, 0.3f);
    scene.setDarkSkyColor(0.1f, 0.1f, 0.1f);

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(-2.0f, 2.0f, 1.0f)));
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(2.0f, -2.0f, 1.0f)));

    /* ---- Objects ---- */
    MeshIOData suzanne;
    read_meshio_data("data/synthese/suzanne.obj", suzanne);
    scene.add(suzanne, translate(-1.0f, 0.2f, -2.0f).rotateY(-10.0f), Color(0.678f, 0.424f, 0.902f), true);
    scene.add(suzanne, translate(1.0f, -0.2f, -2.0f).rotateY(10.0f).rotateZ(180.0f), Color(0.322f, 0.576f, 0.098f),
              false);

    scene.render(width, height);
}

void scene7(unsigned int width, unsigned int height) {
    Scene scene("scene7");

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(0.0f, 0.0f, 1.0f)));

    /* ---- Objects ---- */
    /* Sphere Rings */ {
        unsigned int rings = 32;
        unsigned int spheres = 16;

        float radius = 0.35f;
        float circleRadius = 2.0f;
        float angle = 2.0f * M_PIf / spheres;
        unsigned int hue = 360 / spheres;

        for(unsigned int j = 0 ; j < rings ; ++j) {
            float z = -2.5f - j * (2.0f * radius);

            for(unsigned int i = 0 ; i < spheres ; ++i) {
                scene.add(new Sphere(hueToRGBA(((i + j) % spheres) * hue),
                                     Point(circleRadius * std::cos(i * angle), circleRadius * std::sin(i * angle), z),
                                     radius));
            }

            radius *= 0.9f;
            circleRadius *= 0.9f;
        }
    }

    scene.render(width, height);
}

void scene8(unsigned int width, unsigned int height) {
    Scene scene("scene8");

    /* ---- Sky ---- */
    scene.setLightSkyColor(0.3f, 0.3f, 0.3f);
    scene.setDarkSkyColor(0.1f, 0.1f, 0.1f);

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(0.0f, 2.0f, 1.0f)));

    /* ---- Objects ---- */
    MeshIOData dragon;
    read_meshio_data("data/synthese/dragon80k.obj", dragon);
    scene.add(dragon, translate(0.0f, 0.75f, -4.0f).scale(6.0f).rotateY(75.0f), Red(), false);
    scene.add(dragon, translate(-0.5f, -0.75f, -1.5f).scale(1.5f).rotateY(-75.0f), Blue(), false);
    scene.add(dragon, translate(0.5f, -0.75f, -1.2f).scale(0.5f).rotateY(75.0f), Green(), false);

    scene.render(width, height);
}

void scene9(unsigned int width, unsigned int height) {
    Scene scene("scene9");

    /* ---- Sky ---- */
    scene.setLightSkyColor(0.3f, 0.3f, 0.3f);
    scene.setDarkSkyColor(0.1f, 0.1f, 0.1f);

    /* ---- Lights ---- */
    scene.add(new DirectionalLight(Color(1.0f, 1.0f, 1.0f), Vector(0.0f, -1.0f, 1.0f)));

    /* ---- Objects ---- */
    scene.add("data/synthese/sponza.obj", translate(0.0f, 0.0f, -3.0f).scale(0.05).rotateY(90.0f), White(), false);

    scene.render(width, height);
}

int main() {
    try {
        // scene1(1024, 512);
        // scene2(1024, 512);
        // scene3(1024, 512);
        // scene4(1024, 512);
        // scene5(1024, 512);
        // scene6(768, 512);
        // scene7(512, 512);
        scene8(1024, 1024);
        // scene9(512, 512);
    } catch(const std::exception& exception) {
        std::cerr << "ERROR : " << exception.what() << '\n';
        return -1;
    }

    return 0;
}
