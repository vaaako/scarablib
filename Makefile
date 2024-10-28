CXX = clang++
# I know I could use something like a for loop here, but i prefer this way
CXXFLAGS = -std=c++20 -O3 -ffast-math \
		   -MMD -MP \
		   -I./include \
		   -I./lib/SDL2/include -I./lib/SDL2/include/SDL2 \
		   -I./lib/SDL2_mixer/include \
		   -I./lib/SDL2_ttf/include \
		   -I./lib/glm/include \
		   -I./lib/GLEW/include \
		   -I./lib/stb/include

# -I./lib/SDL2_image/include \

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
TEST_FILE = test/main.cpp

TARGET = scarablib
TARGET_LIB = lib$(TARGET).a

all: $(TARGET)

# Link objects
$(TARGET): $(OBJS)
	mkdir -p $(BUILD_DIR)/temp_objs

	# Initial target library with library objects
	ar rcs $(BUILD_DIR)/$(TARGET_LIB) $(OBJS)

	# Extract object from other libraries
	for lib in lib/*/*.a; do \
		(cd $(BUILD_DIR)/temp_objs && ar x ../../$$lib); \
	done

	# Append and remove
	ar rcs $(BUILD_DIR)/$(TARGET_LIB) $(BUILD_DIR)/temp_objs/*.o
	rm -rf $(BUILD_DIR)/temp_objs

# # Extract object from other libraries
# for lib in lib/*/*.a; do \
# 	cp $$lib $(BUILD_DIR)/$(TARGET_LIB); \
# done
#
# ar rcs $(BUILD_DIR)/$(TARGET_LIB) $(OBJS)



# Compile objects
$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)objects
	$(CXX) $(CXXFLAGS) -c $< -o $@

# cp lib/*/*.a lib/
# $(CXX) -shared -o $(BUILD_DIR)/$@ $^ -Wl,--no-as-needed $(LDFLAGS)
# $(CXX) $(CXXFLAGS) -fPIC -c -o $@ $<

dev: $(TARGET)
	make -f Makefile.dev

clean:
	rm -rf $(BUILD_DIR)/$(TARGET_LIB)
	make clean -f Makefile.dev

.PHONY: all clean dev
