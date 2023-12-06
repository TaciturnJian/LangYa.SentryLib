#!/bin/bash

sudo apt-get update
sudo apt-get install zip wget git cmake build-essential bzip

mkdir ~/repos
cd ~/repos
wget https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.zip
unzip boost_1_83_0.zip
cd boost_1_83_0 
bash ./bootstrap.sh
sudo ./b2 install

