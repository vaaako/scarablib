TARGET=./build/scarablib_dev
FILE=valgrind_output.txt

if [ ! -f $TARGET ]; then
	echo "Target not found! Compile it first"
	exit
fi

valgrind --leak-check=full \
	--show-leak-kinds=all \
	--track-origins=yes \
	--log-file="$FILE" $TARGET
