#!/bin/bash

[ -e build/ ] && rm -r build/*
cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j --target Analyse && \
bin/Analyse