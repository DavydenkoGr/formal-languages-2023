#!/bin/bash

mkdir build_dir && cd build_dir
cmake ../from-regular-to-NFA
cmake --build .

./solution $1 $2 $3
