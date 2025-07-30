cmake_minimum_required(VERSION 3.5)
project(scarablib_dev VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# Set C++ standard and compiler flags
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(USE_STATIC_LIBS "Link against static libraries instead of shared ones" OFF)

file(GLOB_RECURSE SOURCES src/*.cpp src/*/*.cpp src/**/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES} test/main.cpp)


# Mark as private to avoid warnings
target_include_directories(${PROJECT_NAME}
	PRIVATE
		include
	SYSTEM PUBLIC
		include/external
		include/external/GL
		include/external/SDL2
		include/external/enet
		include/external/glm
		include/external/stb
		include/external/tinyobjloader
)

# O0 for debug and fast compiling
target_compile_options(${PROJECT_NAME} PRIVATE -O0 -g -fdiagnostics-color=always -Wextra -Wall -Wuninitialized -Wunreachable-code)

if(USE_STATIC_LIBS)
	link_directories(
		lib/GLEW
		lib/SDL2
		lib/SDL2_mixer
		lib/enet
	)

	# Define the paths to static libraries
	set(LIB_GLEW "${CMAKE_SOURCE_DIR}/lib/GLEW/libGLEW.a")
	set(LIB_SDL2 "${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.a")
	set(LIB_SDL2_MIXER "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.a")
	set(LIB_ENET "${CMAKE_SOURCE_DIR}/lib/enet/libenet.a")

	# Link static libraries
	target_link_libraries(${PROJECT_NAME}
		GL # Link OpenGL dynamically
		${LIB_GLEW}
		${LIB_SDL2}
		${LIB_SDL2_MIXER}
		${LIB_ENET}
	)

else()
	# Set to modern OpenGL
	set(OpenGL_GL_PREFERENCE "GLVND")

	# Find shared libraries
	find_package(OpenGL REQUIRED)

	find_package(SDL2 REQUIRED)
	find_package(SDL2_mixer REQUIRED)
	find_package(GLEW REQUIRED)
	# find_package(enet REQUIRED)
	find_library(ENET_LIBRARY enet PATHS ${CMAKE_SOURCE_DIR}/lib/enet)

	# Link shared libraries
	target_link_libraries(${PROJECT_NAME}
		PRIVATE
		OpenGL::GL
		GLEW::GLEW
		SDL2::SDL2
		SDL2_mixer::SDL2_mixer
		${ENET_LIBRARY}
	)
endif()








