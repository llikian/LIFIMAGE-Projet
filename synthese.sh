#!/bin/bash

[ -e build ] && rm -rf build
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j --target Synthese && bin/Synthese