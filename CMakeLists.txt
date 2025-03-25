# Global Project Configuration
cmake_minimum_required(VERSION 3.26)
project(LIFIMAGE-Projet)

# Compiler options
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_CXX_FLAGS_DEBUG "-ggdb -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Set sources and includes
set(SOURCES
        # Classes
        include/Array2D.hpp

        # Other Sources
        src/utility.cpp

        # Libraries
        lib/gkit3/src/color.cpp
        lib/gkit3/src/files.cpp
        lib/gkit3/src/image_io.cpp
        lib/gkit3/src/mesh_io.cpp
        lib/gkit3/src/vec.cpp
)

set(INCLUDES
        include

        # Libraries
        lib/gkit3/src
)

# Executables
add_executable(Analyse src/analyse.cpp
        ${SOURCES}
        src/analyse/MathematicalMorphology.cpp
        src/analyse/uvec2.cpp
)
target_include_directories(Analyse PUBLIC ${INCLUDES})

add_executable(Synthese src/synthese.cpp
        ${SOURCES}
        src/synthese/Hit.cpp
        src/synthese/Light.cpp
        src/synthese/mat4.cpp
        src/synthese/Object.cpp
        src/synthese/Ray.cpp
        src/synthese/Scene.cpp
        src/synthese/transforms.cpp
        src/synthese/Vertex.cpp
)
target_include_directories(Synthese PUBLIC ${INCLUDES})

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)