CXX=clang++
CXXFLAGS=-std=c++20 -O3 \
		 -I./include/
LDFLAGS=-Llib/scarablib -lscarablib

BUILD_DIR  = build
OBJS_DIR   = $(BUILD_DIR)/objs

SRC_FILES = $(wildcard src/*.cpp src/*/*/*.cpp src/**/*.cpp)
OBJS      = $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(SRC_FILES))

TARGET=testname

all: $(TARGET)

# Make binary
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BUILD_DIR)/$@ $(LDFLAGS)

# Compile cpp files to objs
$(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR) $(BUILD_DIR)/$(TARGET)

.PHONY: all clean

