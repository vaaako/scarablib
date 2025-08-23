#!/bin/bash

TARGET=./build/scarablib_test

FILE=memcheck.log
SUPPRESSIONS=./valgrind.supp
LOG_FILE=memcheck_$(date +%Y%m%d_%H%M%S).log
DEFAULT_OPTS="--leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

if [ ! -f "$TARGET" ]; then
	echo -e "${RED}Error: Target executable not found at '$TARGET'.${NC}"
	echo "Please ensure you have built the project and are running this script from the project's root directory."
	exit 1
fi

# Determine options based on arguments
SUPPRESS=""
case "$1" in
	-s|--suppress)
		SUPPRESS="--suppressions=$SUPPRESSIONS"
		echo -e "${GREEN}Running with suppression file...${NC}"
		;;

	-g|--gen-suppress)
		echo -e "${GREEN}Generating suppression templates...${NC}"

		valgrind --leak-check=full --show-leak-kinds=all \
			--gen-suppressions=all --log-file=/dev/stdout \
			"$TARGET" | tee "${LOG_FILE}_suppressions.txt"

		exit 0
		;;

	-h|--help)
		echo "Usage: $0 [OPTION]"
		echo "Options:"
		echo "  -s, --suppress      Run with suppression file"
		echo "  -g, --gen-suppress  Generate suppression templates"
		echo "  -h, --help          Show this help"
		exit 0
		;;
esac

# Run Valgrind
echo -e "${GREEN}Running Valgrind...${NC}"
echo "Command: valgrind $DEFAULT_OPTS $SUPPRESS --log-file=$LOG_FILE $TARGET"

valgrind $DEFAULT_OPTS $SUPPRESS --log-file="$LOG_FILE" "$TARGET"

# Show summary
echo -e "\n${GREEN}Valgrind analysis complete.${NC}"
echo -e "Log saved to: ${RED}$LOG_FILE${NC}"
echo "View with:"
echo "  less $LOG_FILE"
echo "  grep -A5 'LEAK SUMMARY' $LOG_FILE"
