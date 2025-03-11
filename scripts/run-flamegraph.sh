#!/bin/bash

SECONDS=10

if [ ! -f /usr/bin/flamegraph.pl ] || [ ! -f /usr/bin/stackcollapse-perf.pl ]; then
	echo "Please run ./install-flamegraph.sh first"
fi

if [ $# -ne 1 ]; then
	echo "Usage: ./run-flamegraph.sh <PID>"
	exit
fi

echo "Recording for $SECONDS seconds..."
echo
perf record -p $1 -g -- sleep $SECONDS

echo "Generating flamegraph..."
TIMESTAMP=$(date +"%d%m%y-%H%M%S")

sudo perf script | stackcollapse-perf.pl > out.perf-folded
flamegraph.pl out.perf-folded > $TIMESTAMP.svg

echo "$TIMESTAMP.svg generated"

rm perf.data out.perf-folded
