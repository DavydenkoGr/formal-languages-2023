#!/bin/bash

cmake -S . -B build_dir
cmake --build build_dir
cd build_dir && ctest
gcovr -r ../ .

cd .. && rm -r build_dir
