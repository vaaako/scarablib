cmake_minimum_required(VERSION 3.0)
project(scarablib_dev VERSION 0.0.1)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall -Wuninitialized -Winit-self -Wfloat-equal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=5")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wwrite-strings -Wcast-qual -Wswitch-default")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wunreachable-code")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSCARAB_DEBUG_GLYPHS_LOADED")

# Include directories
include_directories(
	include
	include/external
	include/external/freetype # Freetype needs
)

# Link directories
link_directories(
	lib/GLEW
	lib/SDL2
	lib/SDL2_mixer
	lib/freetype
)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp src/*/*.cpp src/**/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES} test/main.cpp)

# Define the paths to static libraries
set(LIB_GLEW "${CMAKE_SOURCE_DIR}/lib/GLEW/libGLEW.a")
set(LIB_SDL2 "${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.a")
set(LIB_SDL2_MIXER "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.a")
set(LIB_FREETYPE "${CMAKE_SOURCE_DIR}/lib/freetype/libfreetype.a")

# Link static libraries
target_link_libraries(scarablib_dev 
	GL # Link OpenGL dynamically
	# ${LIB_GLEW}
	# ${LIB_SDL2}
	# ${LIB_SDL2_MIXER}
	# ${LIB_FREETYPE}

	GLEW
	SDL2
	SDL2_mixer
	freetype
)
