#!/bin/bash

if [ ! -f /usr/bin/git ]; then
	echo "git is not installed"
	exit 1
fi

if [ ! -f /usr/bin/perf ]; then
	echo "perf is not installed"
	exit 1
fi

# Clone
if [ ! -d FlameGraph ]; then
	git clone https://github.com/brendangregg/FlameGraph
fi

# Copy to bin
sudo cp FlameGraph/stackcollapse-perf.pl /usr/bin/stackcollapse-perf.pl
sudo cp FlameGraph/flamegraph.pl /usr/bin/flamegraph.pl
