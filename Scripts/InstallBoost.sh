#!/bin/bash

echo Updating 
sudo apt-get update
sudo apt-get -y install zip wget curl git cmake build-essential

mkdir ~/repos
cd ~/repos
wget https://github.com/boostorg/boost/releases/download/boost-1.83.0/boost-1.83.0.zip
unzip boost_1_83_0.zip
cd boost_1_83_0 
bash ./bootstrap.sh
sudo ./b2 install

