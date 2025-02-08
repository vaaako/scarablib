CXX = clang++
CXXFLAGS = -std=c++20 -O3 \
		   -I./include \
		   -I./include/external \
		   -I./include/external/freetype

LDFLAGS = -L./lib/GLEW \
		  -L./lib/SDL2 \
		  -L./lib/SDL2_mixer \
		  -L./lib/freetype \
		  -lGL -lGLEW -lSDL2 -lSDL2main -lSDL2_mixer -lfreetype

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

all: shared

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
dev: static
	make -f Makefile.dev

clean:
	rm -rf $(BUILD_DIR)/$(TARGET_LIB) $(OBJS_DIR)
	make clean -f Makefile.dev

.PHONY: all static shared clean
