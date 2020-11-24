#!/bin/bash
echo "removing and recreating build directory...."
rm -rf build
mkdir build
cd build
cmake ..
make
cd ..

