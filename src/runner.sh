#!/usr/bin/env bash

BENCHMARK_DIR=$1
SOLVER=$2
OUTPUT=$3
HEURISTICS=("dlis" "vsids")
MODES=("no-stack" "copy-constructor" "undo-stack")

echo "BENCHMARK DIR: $BENCHMARK_DIR"
if [ "$BENCHMARK_DIR" == "" ]; then
  echo "No benchmark directory given."
  echo "usage:"
  echo "    runner.sh /path/to/benchmark_dir /path/to/solver /path/to/output/data_file"
  exit 1
fi

echo "SOLVER: $SOLVER"
if [ "$SOLVER" == "" ]; then
  echo "No solver given."
  echo "usage:"
  echo "    runner.sh /path/to/benchmark_dir /path/to/solver /path/to/output/data_file"
  exit 1
fi

echo "OUTPUT: $OUTPUT"
if [ "$OUTPUT" == "" ]; then
  echo "No output file given."
  echo "usage:"
  echo "    runner.sh /path/to/benchmark_dir /path/to/solver /path/to/output/data_file"
  exit 1
fi

WORKING_DIR=`pwd`
mkdir -p $WORKING_DIR/tmp
TMP_DIR="$WORKING_DIR/tmp"
# Reset output files.
for heuristic in $HEURISTICS; do
  for mode in $MODES; do
    rm -f $OUTPUT/$heuristic-$mode.csv
    touch $OUTPUT/$heuristic-$mode.csv
  done
done
for BENCHMARK_ARCHIVE in $BENCHMARK_DIR/*.tar.gz; do
  # Clear tmp directory.
  rm -rf $TMP_DIR
  mkdir -p $TMP_DIR
  # Extract benchmarks into tmp directory.
  tar -xzf $BENCHMARK_ARCHIVE -C $TMP_DIR
  # Run solver over all benchmarks and settings in directory.
  file_list=$(find $TMP_DIR -iname '*.cnf' -type f)
  for benchmark_file in $file_list; do
    sed -i -r 's/(\s+)/ /g' $benchmark_file
    for heuristic in $HEURISTICS; do
      for mode in $MODES; do
        echo "$SOLVER \"$heuristic\" \"$mode\" \"$benchmark_file\" 1>$TMP_DIR/tmp_sat_result.txt 2>$TMP_DIR/tmp_time_result.txt"
        $SOLVER "$heuristic" "$mode" "$benchmark_file" >$TMP_DIR/tmp_sat_result.txt 2>$TMP_DIR/tmp_time_result.txt
        #echo "$benchmark_file"
        #echo "--"
        benchmark_name="$(basename $benchmark_file)"
        runtime="$(cat $TMP_DIR/tmp_time_result.txt)"
        satisfiable="$(cat $TMP_DIR/tmp_sat_result.txt)"
        echo "$benchmark_name,$runtime,$satisfiable" >> $OUTPUT/$heuristic-$mode.csv
      done
    done
  done
done
