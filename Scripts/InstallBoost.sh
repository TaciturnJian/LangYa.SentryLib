#!/bin/bash

echo Updating 
sudo apt-get update
sudo apt-get install zip wget curl git cmake build-essential -y

mkdir ~/repos
cd ~/repos
wget https://boostorg.jfrog.io/artifactory/main/release/1.83.0/source/boost_1_83_0.zip
unzip boost_1_83_0.zip
cd boost_1_83_0 
bash ./bootstrap.sh
sudo ./b2 install

