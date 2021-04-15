#!/usr/bin/env bash

BENCHMARK_DIR=$1
SOLVER=$2
OUTPUT=$3
BACKUP_DIR=$4
HEURISTICS=("dlis" "vsids")
MODES=("no-stack" "copy-constructor" "undo-stack")

echo "BENCHMARK DIR: $BENCHMARK_DIR"
if [ "$BENCHMARK_DIR" == "" ]; then
  echo "No benchmark directory given."
  echo "usage:"
  echo "    runner.sh /path/to/benchmark_dir /path/to/solver /path/to/output/data_file /path/to/backup/dir"
  exit 1
fi

echo "SOLVER: $SOLVER"
if [ "$SOLVER" == "" ]; then
  echo "No solver given."
  echo "usage:"
  echo "    runner.sh /path/to/benchmark_dir /path/to/solver /path/to/output/data_file /path/to/backup/dir"
  exit 1
fi

echo "OUTPUT: $OUTPUT"
if [ "$OUTPUT" == "" ]; then
  echo "No output file given."
  echo "usage:"
  echo "    runner.sh /path/to/benchmark_dir /path/to/solver /path/to/output/data_file /path/to/backup/dir"
  exit 1
fi

echo "BACKUP_DIR: $BACKUP_DIR"
if [ "$BACKUP_DIR" == "" ]; then
  echo "No backup directory given."
  echo "usage:"
  echo "    runner.sh /path/to/benchmark_dir /path/to/solver /path/to/output/data_file /path/to/backup/dir"
  exit 1
fi
WORKING_DIR=`pwd`
mkdir -p $WORKING_DIR/tmp
TMP_DIR="$WORKING_DIR/tmp"
# Reset output files.
for heuristic in "${HEURISTICS[@]}"; do
  for mode in "${MODES[@]}"; do
    rm -f $OUTPUT/$heuristic-$mode.csv
    touch $OUTPUT/$heuristic-$mode.csv
  done
done
# Create logfile for commands that have been run.
LOGFILE="$OUTPUT/command.log"
rm -f $LOGFILE
touch $LOGFILE

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
    for heuristic in "${HEURISTICS[@]}"; do
      for mode in "${MODES[@]}"; do
        echo "$SOLVER \"$heuristic\" \"$mode\" \"$benchmark_file\" 1>$TMP_DIR/tmp_sat_result_${heuristic}_${mode}.txt 2>$TMP_DIR/tmp_time_result_${heuristic}_${mode}.txt" >> "$LOGFILE"
        $SOLVER "$heuristic" "$mode" "$benchmark_file" >$TMP_DIR/tmp_sat_result_${heuristic}_${mode}.txt 2>$TMP_DIR/tmp_time_result_${heuristic}_${mode}.txt &
      done
    done
    wait
    for heuristic in "${HEURISTICS[@]}"; do
      for mode in "${MODES[@]}"; do
        benchmark_name="$(basename $benchmark_file)"
        runtime="$(cat $TMP_DIR/tmp_time_result_${heuristic}_${mode}.txt)"
        satisfiable="$(cat $TMP_DIR/tmp_sat_result_${heuristic}_${mode}.txt)"
        echo "$benchmark_name,$runtime,$satisfiable" >> $OUTPUT/$heuristic-$mode.csv
      done
    done
  done
  for heuristic in "${HEURISTICS[@]}"; do
    for mode in "${MODES[@]}"; do
      cp $LOGFILE $BACKUP_DIR/command.log
      cp $OUTPUT/$heuristic-$mode.csv $BACKUP_DIR/$heuristic-$mode.csv
    done
  done
done
