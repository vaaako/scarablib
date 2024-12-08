cmake_minimum_required(VERSION 3.0)
project(scarablib VERSION 0.0.1)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -g -fdiagnostics-color=always -Wextra -Wall -Wuninitialized -Winit-self -Wfloat-equal -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=5 -Wwrite-strings -Wcast-qual -Wswitch-default -Wconversion -Wunreachable-code")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

link_directories(
	lib/GLEW
	lib/SDL2
	lib/SDL2_mixer
	lib/SDL2_ttf
)

add_definitions(-DSCARAB_DEBUG_VAO -DSCARAB_DEBUG_DRAWCALL)
file(GLOB_RECURSE SOURCES src/*.cpp src/*/*.cpp src/**/*.cpp)
add_executable(scarablib_dev ${SOURCES} test/main.cpp)
target_link_libraries(scarablib_dev GL GLEW SDL2 SDL2main SDL2_mixer SDL2_ttf)
