CXX = clang++
CXXFLAGS = -std=c++20 -O3 \
		   -MMD -MP \
		   -I./include \
		   -I./include/GLEW \
		   -I./include/glm \
		   -I./include/SDL2 -I./include/SDL2/SDL2 \
		   -I./include/SDL2_mixer \
		   -I./include/SDL2_ttf \
		   -I./include/stb

DEVFLAGS = -g \
		   -fdiagnostics-color=always \
		   -Wall \
		   -Wextra \
		   -Wuninitialized \
		   -Winit-self \
		   -Wfloat-equal \
		   -Wundef \
		   -Wpointer-arith \
		   -Wcast-align \
		   -Wstrict-overflow=5 \
		   -Wwrite-strings \
		   -Wcast-qual \
		   -Wswitch-default \
		   -Wconversion \
		   -Wunreachable-code

# -I./lib/SDL2_image/include \

LDFLAGS = -L./lib/GLEW \
		  -L./lib/SDL2 \
		  -L./lib/SDL2_mixer \
		  -L./lib/SDL2_ttf \
		  -lGL -lGLEW -lSDL2 -lSDL2main -lSDL2_mixer -lSDL2_ttf

# Dirs
BUILD_DIR = build
OBJS_DIR = $(BUILD_DIR)/objs
#SHAREDOBJS_DIR = $(BUILD_DIR)/sharedobjs

# Files
SRC_FILES = $(wildcard src/*.cpp src/*/*/*.cpp src/**/*.cpp)
OBJS = $(patsubst src/%.cpp, $(OBJS_DIR)/%.o, $(SRC_FILES))
TEST_FILE = test/main.cpp

TARGET = scarablib
TARGET_STATIC = lib$(TARGET).a
TARGET_SHARED = lib$(TARGET).so

all: $(TARGET)

# Link objects
$(TARGET): $(OBJS)
	mkdir -p $(BUILD_DIR)/temp_objs

	echo "Making static"

	# Initial target library with library objects
	ar rcs $(BUILD_DIR)/$(TARGET_STATIC) $(OBJS)

	# Extract object from other libraries
	for lib in lib/*/*.a; do \
		(cd $(BUILD_DIR)/temp_objs && ar x ../../$$lib); \
	done

	# Append and remove
	ar rcs $(BUILD_DIR)/$(TARGET_STATIC) $(BUILD_DIR)/temp_objs/*.o
	rm -rf $(BUILD_DIR)/temp_objs

	# echo "Making shared"
	# $(CXX) $(CXXFLAGS) -fPIC -shared $(OBJS) -o lib$(TARGET).so

# Compile objects
$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@
	# Make shared
	# $(CXX) $(CXXFLAGS) -c $< -o $@


# Append warnings
dev: CXXFLAGS += $(DEVFLAGS)
dev: $(TARGET)
	make -f Makefile.dev

clean:
	rm -rf $(BUILD_DIR)/$(TARGET_LIB) $(OBJS_DIR)
	make clean -f Makefile.dev

.PHONY: all clean dev
