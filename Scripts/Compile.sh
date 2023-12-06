#!/bin/bash

rm build -rf
mkdir build
cd build
cmake ..
make -j8
cd ..
