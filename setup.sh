#!/bin/bash

# Print a warning in red color to only run this script from inside the scripts directory
echo -e "\e[33mWARNING: Only run this script from main directory! (type 1 or 2)\e[0m"
PS3="Are you sure you want to continue? "
options=("Yes" "No")
select opt in "${options[@]}"; do
    case $opt in
    "Yes")
        break
        ;;
    "No")
        # Exit script but do not exit terminal
        return 0
        ;;
    *) echo "Invalid option" ;;
    esac
done

source /cvmfs/sft.cern.ch/lcg/views/LCG_100/x86_64-centos7-gcc8-opt/setup.sh
source /cvmfs/sft.cern.ch/lcg/views/LCG_100/x86_64-centos7-gcc8-opt/bin/geant4.sh
echo -e "\e[32mSourced Geant4\e[0m"

# Make directory for results
mkdir -p results
echo -e "\e[32mCreated directory for results\e[0m"

# Check if directory data exists
if [ ! -d "data" ]; then
    # If directory data does not exist, create symbolic link
    ln -s /pc/choozdsk01/DATA/CAJITA/data $PWD
    echo -e "\e[32mCreated symbolic link for data\e[0m"
    echo -e "\e[32mSetup complete!\e[0m"

else
    echo -e "\e[33mWARNING: Data folder exists. (type 1 or 2)\e[0m"
    PS3="Do you want to delete it? "
    options=("Yes" "No")
    select opt in "${options[@]}"; do
        case $opt in
        "Yes")
            rm -r data
            echo -e "\e[31mDeleted data folder\e[0m"
            break
            ;;
        "No")
            break
            ;;
        *) echo "Invalid option" ;;
        esac
    done
fi

ln -s /pc/choozdsk01/DATA/CAJITA/data $PWD
echo -e "\e[32mCreated symbolic link for data\e[0m"
echo -e "\e[32mSetup complete!\e[0m"
