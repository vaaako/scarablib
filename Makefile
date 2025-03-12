CXX = clang++
CXXFLAGS = -std=c++20 -O3 \
		   -I./include \
		   -I./include/external/SDL2 \
		   -I./include/external/SDL_mixer \
		   -Iinclude/external/tinyobjloader \
		   -Iinclude/external/stb
# -I./include/external/SDL2 -> SDL_mixer

# Default to 'static' if not specified
LIBRARY_TYPE ?= static

# Conditional LDFLAGS based on LIBRARY_TYPE
ifeq ($(LIBRARY_TYPE), static)
	LDFLAGS = ./lib/GLEW/libGLEW.a \
			  ./lib/SDL2/libSDL2.a \
			  ./lib/SDL2_mixer/libSDL2_mixer.a \
			  -lGL -ldl -lpthread -lm
else ifeq ($(LIBRARY_TYPE), shared)
	LDFLAGS = -L./lib/GLEW \
			  -L./lib/SDL2 \
			  -L./lib/SDL2_mixer \
			  -lGL -lGLEW -lSDL2 -lSDL2main -lSDL2_mixer
endif

# Dirs
BUILD_DIR = build
OBJS_DIR = $(BUILD_DIR)/objs
DEPS_DIR = $(BUILD_DIR)/deps

# Files
SRC_FILES = $(wildcard src/*.cpp src/*/*/*.cpp src/**/*.cpp)
OBJS = $(patsubst src/%.cpp, $(OBJS_DIR)/%.o, $(SRC_FILES))
DEPS = $(patsubst src/%.cpp, $(DEPS_DIR)/%.d, $(SRC_FILES))

# Library targets
TARGET = scarablib
TARGET_STATIC = $(BUILD_DIR)/lib$(TARGET).a
TARGET_SHARED = $(BUILD_DIR)/lib$(TARGET).so

all: $(LIBRARY_TYPE)

# Static library
static: $(TARGET_STATIC)
$(TARGET_STATIC): $(OBJS)
	@echo "Building static library"
	@mkdir -p $(BUILD_DIR)/temp_objs
	# Initial target library with library objects
	ar rcs $@ $(OBJS)
	# Extract object from other libraries
	for lib in lib/*/*.a; do \
		(cd $(BUILD_DIR)/temp_objs && ar x ../../$$lib); \
	done
	# Append and remove
	ar rcs $@ $(BUILD_DIR)/temp_objs/*.o
	rm -rf $(BUILD_DIR)/temp_objs

# Shared Library
shared: CXXFLAGS += -fPIC
shared: $(TARGET_SHARED)
$(TARGET_SHARED): $(OBJS)
	@echo "Building shared library"
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -shared $(OBJS) -o $@ $(LDFLAGS)

# Compile objects
$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Generate dependencies
$(DEPS_DIR)/%.d: src/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,$(OBJS_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

# Include dependencies
-include $(DEPS)


# Test with static
# TODO -- change to shared later
# dev: static
# 	make -f Makefile.dev

clean:
	rm -rf $(OBJS_DIR) $(DEPS_DIR)

.PHONY: all static shared clean
