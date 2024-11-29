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

LDFLAGS = -L./lib/GLEW \
		  -L./lib/SDL2 \
		  -L./lib/SDL2_mixer \
		  -L./lib/SDL2_ttf \
		  -lGL -lGLEW -lSDL2 -lSDL2main -lSDL2_mixer -lSDL2_ttf

# Dirs
BUILD_DIR = build
OBJS_DIR = $(BUILD_DIR)/objs

# Files
SRC_FILES = $(wildcard src/*.cpp src/*/*/*.cpp src/**/*.cpp)
OBJS = $(patsubst src/%.cpp, $(OBJS_DIR)/%.o, $(SRC_FILES))

# Library targets
TARGET = scarablib
TARGET_STATIC = lib$(TARGET).a
TARGET_SHARED = lib$(TARGET).so

all: static shared

static: $(OBJS)
	@echo "Building static library"
	@mkdir -p $(BUILD_DIR)/temp_objs

	# Initial target library with library objects
	ar rcs $(BUILD_DIR)/$(TARGET_STATIC) $(OBJS)

	# Extract object from other libraries
	for lib in lib/*/*.a; do \
		(cd $(BUILD_DIR)/temp_objs && ar x ../../$$lib); \
	done

	# Append and remove
	ar rcs $(BUILD_DIR)/$(TARGET_STATIC) $(BUILD_DIR)/temp_objs/*.o
	rm -rf $(BUILD_DIR)/temp_objs

shared: CXXFLAGS += -fPIC
shared: $(OBJS)
	@echo "Building static library"
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -shared $(OBJS) -o $(BUILD_DIR)/$(TARGET_SHARED) $(LDFLAGS)

# Compile objects
$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@


dev: $(TARGET)
	make -f Makefile.dev

clean:
	rm -rf $(BUILD_DIR)/$(TARGET_LIB) $(OBJS_DIR)
	make clean -f Makefile.dev

.PHONY: all clean dev
