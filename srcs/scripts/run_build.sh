#!/bin/bash

# Print a warning in red color
echo -e "\e[31mWARNING: Only run this script from srcs directory!\e[0m"

# Ask for confirmation
read -p "Are you sure you want to continue? (y/n) " -n 1 -r
echo
[[ ! $REPLY =~ ^[Yy]$ ]] && exit 1

# Create and clean build directory
mkdir -p ../build
rm -rf ../build/*

# Check if an argument is provided
if [ $# -eq 0 ]; then
    echo "\n -->Choose a geometry file from the list below (without extension):"
    ls geometries/*.cc | sed 's|geometries/||g' | sed 's|\.cc$||g'
    read -p "Enter the name of the geometry file: " geometry_file
else
    geometry_file=$1
    echo -e "\e[34m\nRunning with input argument:\e[0m $geometry_file\n"
fi

# Copy selected geometry file and exit if it fails
cp geometries/$geometry_file.cc construction.cc
cp geometries/$geometry_file.hh construction.hh || { echo -e "\e[31m\nError: Failed to copy files. Exiting.\e[0m"; exit 1; }

# Run CMake and build
cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B ../build/
cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B ../build/ -DGEANT4_BUILD_MULTITHREADED=ON
cmake --build ../build/ --config Release -- -j 8