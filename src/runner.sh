#!/usr/bin/env bash

while getopts u:a:f: flag
do
    case "${flag}" in
        b) BENCHMARK_DIR=${OPTARG};;
        s) SOLVER=${OPTARG};;
    esac
done

BENCHMARKS=$BENCHMARK_DIR/*.tar.gz
