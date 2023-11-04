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

source /cvmfs/sft.cern.ch/lcg/views/LCG_100/x86_64-centos7-gcc8-opt/setup.sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_100/x86_64-centos7-gcc8-opt/bin/geant4.sh

# Make directory for results
mkdir -p results

# Check if directory data exists
if [ ! -d "data" ]; then
    mkdir data
    sshfs manthey@gaeuidc1.ciemat.es:/pc/choozdsk01/palomare/GEANT4/ data
fi