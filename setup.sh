#!/bin/bash

echo "BASH : define environment variables necessary for ROOT setup"
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
export ALRB_localConfigDir=$HOME/localConfig
echo "BASH : set up cvmfs ROOT"
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
source ${ATLAS_LOCAL_ROOT_BASE}/packageSetups/atlasLocalROOTSetup.sh --rootVer=6.04.14-x86_64-slc6-gcc49-opt --skipConfirm

# Required for setting up Delphes libarary
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/users/jwebster/Delphes-3.3.0

# Required for processing ProMC files
export PROMC=/share/sl6/promc
export PATH=$PROMC/bin:$PATH
export PATH=/share/sl6/cmake/bin/:$PATH


# define some terminal font styles
# (these may only work when running interactively)
#
#
echo "BASH : define some terminal font styles"
#
#
export FONTNONE='\033[00m'
export FONTRED='\033[01;31m'
export FONTGREEN='\033[01;32m'
export FONTYELLOW='\033[01;33m'
export FONTPURPLE='\033[01;35m'
export FONTCYAN='\033[01;36m'
export FONTWHITE='\033[01;37m'
export FONTBOLD='\033[1m'
export FONTUNDERLINE='\033[4m'

echo "BASH : done."
