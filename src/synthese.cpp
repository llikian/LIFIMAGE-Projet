/***************************************************************************************************
 * @file  synthese.cpp
 * @brief Contains the main program for the 'synthese' executable
 **************************************************************************************************/

#include "synthese/Application.hpp"

#include <iostream>
#include <stdexcept>
#include "image_io.h"

int main() {
    try {
        Application application;
        Image image = application.run(512, 512);
        write_image(image, "data/synthese/img.png");
    } catch(const std::exception& exception) {
        std::cerr << "ERROR : " << exception.what() << '\n';
        return -1;
    }

    return 0;
}