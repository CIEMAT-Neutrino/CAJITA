#!/bin/bash

# Print a warning in red color to only run this script from inside the scripts directory
echo -e "\e[31mWARNING: Only run this script from main directory!\e[0m"
# Ask if sure to continue
read -p "Are you sure you want to continue? (y/n) " -n 1 -r
echo
# If the user did not answer with y, exit the script
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    exit 1
fi

# Create build and data(output) directory if not present
if [ ! -d "build/" ]; then
    mkdir build/
fi

if [ ! -d "data/" ]; then
    mkdir data/
fi

# Remove contents of build folder if any
rm -rf build/*

# Check if argument provided
if [ $# -eq 0 ]; then
    # Ask the user for a geometry file to choose from the contents of the geometries directory
    echo "Choose a geometry file from the list below:"
    # Format the list of files to be more readable for the user. Only show the file name without the directory path
    ls geometries/*.cc | sed 's/geometries\///g'
    # Ask the user for the name of the geometry file
    read -p "Enter the name of the geometry file: " geometry_file
    # Copy the chosen geometry file to the main directory and rename it to construction.cc
    cp geometries/$geometry_file.cc construction.cc
    cp geometries/$geometry_file.hh construction.hh

    cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B build/
    cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B build/ -DGEANT4_BUILD_MULTITHREADED=ON
    cmake --build build/ --config Release -- -j 8

else
    echo -e "\e[34mRunning with input argument:\e[0m $1" 
    echo

    cp geometries/$1.cc construction.cc
    cp geometries/$1.hh construction.hh

    cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B build/
    cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B build/ -DGEANT4_BUILD_MULTITHREADED=ON
    cmake --build build/ --config Release -- -j 8
fi