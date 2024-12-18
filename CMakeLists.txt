cmake_minimum_required(VERSION 3.0)
project(scarablib VERSION 0.0.1)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -fdiagnostics-color=always -Wextra -Wall -Wuninitialized -Winit-self -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wconversion -Wunreachable-code")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include directories
include_directories(
    include
    include/GLEW
    include/SDL2
	include/SDL2/SDL2 # For SDL2 libraries
    include/SDL2_mixer
    include/SDL2_ttf
    include/glm
    include/stb
)

# Define the paths to static libraries
set(LIB_GLEW "${CMAKE_SOURCE_DIR}/lib/GLEW/libGLEW.a")
set(LIB_SDL2 "${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.a")
set(LIB_SDL2_MIXER "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.a")
set(LIB_SDL2_TTF "${CMAKE_SOURCE_DIR}/lib/SDL2_ttf/libSDL2_ttf.a")

# Define sources
file(GLOB_RECURSE SOURCES src/*.cpp src/*/*.cpp src/**/*.cpp)

# Add executable
add_executable(scarablib_dev ${SOURCES} test/main.cpp)

# Link static libraries
target_link_libraries(scarablib_dev 
    ${LIB_GLEW}
    ${LIB_SDL2}
    ${LIB_SDL2_MIXER}
    ${LIB_SDL2_TTF}
    GL # Link OpenGL dynamically
)

