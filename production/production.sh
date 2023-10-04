#!/bin/bash

### RUN from SIM_LAB_GEANT4 folder with: sh production/production.sh ###

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

time ./build/box1 production/megacell_40mm_prod.json /pc/choozdsk01/palomare/GEANT4/megacell_v2/megacell_40mm_1cm.root
time ./build/box1 production/megacell_45mm_prod.json /pc/choozdsk01/palomare/GEANT4/megacell_v2/megacell_45mm_1cm.root
time ./build/box1 production/megacell_50mm_prod.json /pc/choozdsk01/palomare/GEANT4/megacell_v2/megacell_50mm_1cm.root