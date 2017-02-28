#!/bin/bash

if [ $# == 2 ]; then
  echo "Error, run \`$0 path_to_executable\`"
  echo ""
  exit 1
fi

MIN_ITER=1000
MAX_ITER=1100
MAX_SEC=60
MIN_SIZE=20
STEP_SIZE=1.2

echo "benchmarking for llong"
$1 $MIN_ITER $MAX_ITER $MAX_SEC $MIN_SIZE $STEP_SIZE l > report_llong.csv
echo "benchmarking for int"
$1 $MIN_ITER $MAX_ITER $MAX_SEC $MIN_SIZE $STEP_SIZE i > report_int.csv
echo "benchmarking for double"
$1 $MIN_ITER $MAX_ITER $MAX_SEC $MIN_SIZE $STEP_SIZE d > report_double.csv
echo "benchmarking for float"
$1 $MIN_ITER $MAX_ITER $MAX_SEC $MIN_SIZE $STEP_SIZE f > report_float.csv

echo "Doing visualizations"
python3 visualize.py report_llong.csv
python3 visualize.py report_int.csv
python3 visualize.py report_double.csv
python3 visualize.py report_float.csv

echo "To contribute, make another folder in out,"
echo "copy results there, write appropriate"
echo "RESULTS.md and link to if from README.md"

