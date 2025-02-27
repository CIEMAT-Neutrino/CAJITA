#!/bin/bash

# Print a warning in red color to only run this script from inside the scripts directory
echo -e "\e[31mWARNING: Only run this script from main directory!\e[0m"
read -p "Are you sure you want to continue? (y/n) " -n 1 -r

# If the user did not answer with y, exit the script
if [[ ! $REPLY =~ ^[Yy]$ ]]
then
    exit 1
fi

source /cvmfs/sft.cern.ch/lcg/views/LCG_100/x86_64-centos7-gcc8-opt/setup.sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_100/x86_64-centos7-gcc8-opt/bin/geant4.sh
echo -e "\e[32mSourced Geant4\e[0m"

# Make directory for results
mkdir -p results
echo -e "\e[32mCreated directory for results\e[0m"

# Check if directory data exists
if [ ! -d "data" ]; then
    mkdir data
    sshfs $USER@gaeuidc1.ciemat.es:/pc/choozdsk01/DATA/CAJITA/ data
    echo -e "\e[32mCreated directory for data\e[0m"
fi
echo -e "\e[32mSetup complete!\e[0m"