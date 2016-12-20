#!/bin/bash

SOURCEDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

if [[ -z ${1+x} ]]; then
  echo "SiliconAngleSim_batch.sh output-directory config-yaml [extra-options]";
  exit -1
fi

if [[ -z ${2+x} ]]; then
  echo "Config file is required!";
  exit -2
fi

# Prepare ATLAS environment
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${ATLAS_LOCAL_ROOT_BASE}/user/atlasLocalSetup.sh --quiet
lsetup root

# Setup SiliconAngleSim paths
source ${SOURCEDIR}/setup_SiliconAngleSim.sh

# Change to working directory
pushd $1 > /dev/null

# Run job
SiliconAngleSim $2 $3
