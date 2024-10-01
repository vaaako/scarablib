if [ ! -f /usr/bin/bear ]; then
	echo "bear not found, please install it first"
	exit
fi

if [ ! -d src ]; then
	echo "Please run this script from root"
	exit
fi

bear --output build/compile_commands.json -- make dev
