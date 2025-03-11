cmake_minimum_required(VERSION 3.0)
project(scarablib_dev VERSION 0.0.1)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fdiagnostics-color=always")
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -MM -MD -fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wall -Wuninitialized -Winit-self -Wfloat-equal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wundef -Wpointer-arith -Wcast-align -Wstrict-overflow=5")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wwrite-strings -Wcast-qual -Wswitch-default")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wunreachable-code")

option(USE_STATIC_LIBS "Link against static libraries instead of shared ones" OFF)

file(GLOB_RECURSE SOURCES src/*.cpp src/*/*.cpp src/**/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES} test/main.cpp)

if(USE_STATIC_LIBS)
	# Include directories
	include_directories(
		include
		include/external
		include/external/SDL2 # SDL_mixer includes SDL_stdinc
		include/external/SDL_mixer
		include/external/tinyobjloader
		include/external/stb
	)

	link_directories(
		lib/GLEW
		lib/SDL2
		lib/SDL2_mixer
		lib/freetype
	)

	# Define the paths to static libraries
	set(LIB_GLEW "${CMAKE_SOURCE_DIR}/lib/GLEW/libGLEW.a")
	set(LIB_SDL2 "${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.a")
	set(LIB_SDL2_MIXER "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.a")
	set(LIB_FREETYPE "${CMAKE_SOURCE_DIR}/lib/freetype/libfreetype.a")

	# Link static libraries
	target_link_libraries(scarablib_dev 
		GL # Link OpenGL dynamically
		${LIB_GLEW}
		${LIB_SDL2}
		${LIB_SDL2_MIXER}
		${LIB_FREETYPE}
	)

else()

	# Set to modern OpenGL
	set(OpenGL_GL_PREFERENCE "GLVND")

	# Include directories
	include_directories(
		include
		include/external
		include/external/SDL2 # SDL_mixer includes SDL_stdinc
		include/external/SDL_mixer
		include/external/tinyobjloader
		include/external/stb
	)

	# Find shared libraries
	find_package(OpenGL REQUIRED)

	find_library(SDL2_LIBRARY SDL2 PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2)
	find_library(SDL2_MIXER_LIBRARY SDL2_mixer PATHS ${CMAKE_SOURCE_DIR}/lib/SDL2_mixer)
	find_library(GLEW_LIBRARY GLEW PATHS ${CMAKE_SOURCE_DIR}/lib/GLEW)

	# Link shared libraries
	target_link_libraries(scarablib_dev 
		${OPENGL_LIBRARIES}
		${GLEW_LIBRARY}
		${SDL2_LIBRARY}
		${SDL2_MIXER_LIBRARY}
	)
endif()








