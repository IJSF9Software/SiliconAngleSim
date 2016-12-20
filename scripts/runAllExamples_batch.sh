#!/bin/bash

SOURCEDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

BINARY_PATH="/afs/cern.ch/work/t/tadej/public/root/bin"
OUTPUT_PATH="${SOURCEDIR}/../output"
TASK="SiliconAngleSim_batch.sh"
EXAMPLES_PATH="${SOURCEDIR}/../examples"

for f in `find $EXAMPLES_PATH -type f \( -iname '*.yaml*' \)`; do
  name=${f//${EXAMPLES_PATH}\//}
  name=${name//\.yaml/}

  echo "=== Submitting $name..."
  bsub -J $name -q 1nh $BINARY_PATH/$TASK $OUTPUT_PATH $f -s
done
