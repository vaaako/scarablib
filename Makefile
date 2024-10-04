CXX = clang++
CXXFLAGS_DEV = -g \
			   -fdiagnostics-color=always \
			   -Wextra \
			   -Wall \
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

# -L./lib/SDL2_image \

# Dirs
BUILD_DIR = build
OBJS_DIR = $(BUILD_DIR)/objs

# Files
SRC_FILES = $(wildcard src/*.cpp src/*/*/*.cpp src/**/*.cpp)
OBJS = $(patsubst src/%.cpp, $(OBJS_DIR)/%.o, $(SRC_FILES))
TEST_FILE = test/main.cpp

TARGET = scarablib

all: $(TARGET)

# Link objects
$(TARGET): $(OBJS)
	@echo "nothing here yet"
# $(CXX) -shared -o $(BUILD_DIR)/$@ $^ -Wl,--no-as-needed $(LDFLAGS)
# $(CXX) $(CXXFLAGS) -fPIC -c -o $@ $<


# Apply dev flags to target before
dev: CXXFLAGS += $(CXXFLAGS_DEV)
dev: $(OBJS)
	$(CXX) $(CXXFLAGS) $^ $(TEST_FILE) -o $(BUILD_DIR)/$(TARGET)_dev $(LDFLAGS)

# Compile each object
$(OBJS_DIR)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/$(TARGET) $(OBJS_DIR)

.PHONY: all clean dev
