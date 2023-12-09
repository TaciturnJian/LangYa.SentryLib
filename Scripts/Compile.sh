#!/bin/bash

[[ -d build ]] && \
echo "Trying to remove build directory" && \
rm build -rf

mkdir build
[[ -d build ]] && \
echo "Created build directory" && \
cd build && \
cmake .. && \
make -j8 && \
cd .. && \
exit 0

echo "Cannot find build directory after executed mkdir!"
exit -1


