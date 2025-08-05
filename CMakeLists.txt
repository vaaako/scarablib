cmake_minimum_required(VERSION 3.10)
project(scarablib VERSION 0.1.0)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

# -- Project settings
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# -- Headers
option(STATIC "Buil as a static library" ON)
option(SHARED "Build as a shared library" OFF)
option(DEBUG "Build with debug flags" OFF)

# -- Source files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	# This is better than compiling to a static library
	lib/glad/glad2.c
	src/*.cpp src/*/*.cpp src/**/*.cpp)

# -- Compile to static and/or shared libraries
if(STATIC)
	add_library(${PROJECT_NAME} STATIC ${SOURCES})
endif()

if(SHARED)
	add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif()

# -- Mark external headers as PUBLIC to avoid warnings
target_include_directories(${PROJECT_NAME}
	PRIVATE
		include
	PUBLIC
		include/external
		include/external/glad
		include/external/SDL2
		include/external/enet
		include/external/glm
		include/external/stb
		include/external/tinyobjloader
)

# -- Target test executable
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(TEST_NAME "scarablib_test")
	add_executable(${TEST_NAME} test/main.cpp)
	target_link_libraries(${TEST_NAME} PRIVATE ${PROJECT_NAME}) # Link the compiled library
	target_include_directories(${TEST_NAME} PRIVATE include)
endif()


# -- Compile flags
# -O0 for debug and fast compiling
target_compile_options(${PROJECT_NAME} PRIVATE
	$<$<CONFIG:Debug>:
		-O0 -g -fdiagnostics-color=always
		-Wextra -Wall -Wuninitialized -Wunreachable-code
	>
	$<$<CONFIG:Release>:
		# -march=native -> Optimize for current CPU (may not work on all CPUs)
		# -DNDEBUG -> Remove debug symbols
		-O3 -march=native -ffast-math -DNDEBUG
	>
)

# -- Linking static libraries
set(OpenGL_GL_PREFERENCE "GLVND") # Set to modern OpenGL

# Windows compilation was not tested
if(WIN32)
	# Locate SDL2 and its components
	find_package(SDL2 REQUIRED CONFIG)
	find_package(SDL2_mixer REQUIRED CONFIG)

	target_link_libraries(${PROJECT_NAME}
		SDL2::SDL2-static
		SDL2::SDL2main
		SDL2_mixer::SDL2_mixer-static
		# System libraries required by SDL2 on windows
		opengl32
		winmm
		imm32
		version
	)
elseif(UNIX)
	target_link_libraries(${PROJECT_NAME}
		GL # Link OpenGL dynamically
		"${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.a"
		"${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.a"
	)
endif()
