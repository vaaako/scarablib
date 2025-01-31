#!/bin/bash

TARGET=./build/scarablib_dev
FILE=memcheck.log

# Check if the target exists
if [ ! -f "$TARGET" ]; then
	echo "Target not found! Compile it first."
	exit 1
fi

# Run Valgrind with or without suppressions based on the argument
if [ "$1" = "-s" ]; then
	echo "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=valgrind.supp --log-file=$FILE $TARGET"
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--suppressions=valgrind.supp \
		--log-file="$FILE" "$TARGET"
		# --gen-suppressions=all --log-file=memcheck.log "$TARGET"
else
	echo "valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=$FILE $TARGET"
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--gen-suppressions=all --log-file=memcheck.log "$TARGET"
		# --log-file="$FILE" "$TARGET"
fi
