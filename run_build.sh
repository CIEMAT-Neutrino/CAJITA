#!/bin/bash
#create build and data(output) directory if not present
(
    if [ ! -d "build" ]; then
    mkdir build
    fi
) &&
(
    if [ ! -d "data" ]; then
    mkdir data
    fi
) &&
(rm -rf build/*) &&
(cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B build/) &&
(cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B build/ -DGEANT4_BUILD_MULTITHREADED=ON ) &&
(cmake --build build/ --config Release -- -j 8)
