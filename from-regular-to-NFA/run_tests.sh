#!/bin/bash

rm -r build_dir && mkdir build_dir

cmake -S . -B build_dir
cmake --build build_dir
cd build_dir && ctest
