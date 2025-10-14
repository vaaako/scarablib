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

	# Trully link static library
	if(UNIX)
		# -- Linking static libraries
		set(OpenGL_GL_PREFERENCE "GLVND") # Set to modern OpenGL
		target_link_libraries(${PROJECT_NAME} PUBLIC
			GL      # OpenGL
			pthread # Ensure thread safe
			m       # Math functions
			dl      # Dynamic loader
		)

		# SDL2 as IMPORTED STATIC library
		add_library(SDL2 STATIC IMPORTED)
		set_target_properties(SDL2 PROPERTIES
			IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.a"
		)

		# SDL2_mixer as IMPORTED STATIC library
		add_library(SDL2_mixer STATIC IMPORTED)
		set_target_properties(SDL2_mixer PROPERTIES
			IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.a"
		)

		add_custom_command(TARGET scarablib POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E echo "Merging SDL2 and SDL2_mixer into ${PROJECT_NAME}"
			COMMAND ${CMAKE_AR} x ${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.a
			COMMAND ${CMAKE_AR} x ${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.a
			COMMAND ${CMAKE_AR} qcs $<TARGET_FILE:scarablib> *.o
			COMMAND ${CMAKE_COMMAND} -E rm -f *.o
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		)
	endif()

	if(WIN32)
		# --- Windows (MinGW/Clang) ---
		target_link_libraries(${PROJECT_NAME} PUBLIC
			"${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.dll"
			opengl32
			winmm
			imm32
			version
			ole32
			gdi32
		)

		# prevent SDL from redefining main()
		target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)

		# Merge SDL2 + SDL2_mixer if you have true static .a builds
		add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E echo "Merging SDL2 and SDL2_mixer into ${PROJECT_NAME}"
			COMMAND ${CMAKE_AR} x ${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.lib
			COMMAND ${CMAKE_AR} qcs $<TARGET_FILE:${PROJECT_NAME}> *.o
			COMMAND ${CMAKE_COMMAND} -E rm -f *.o
			WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
		)
	endif()
endif()

if(SHARED)
	add_library(${PROJECT_NAME} SHARED ${SOURCES})

	if(UNIX)
		target_link_libraries(${PROJECT_NAME} PUBLIC
			GL      # OpenGL
			pthread # Ensure thread safe
			m       # Math functions
			dl      # Dynamic loader
			"${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.so"
			"${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.so"
		)
	endif()

	if(WIN32)
		target_link_libraries(${PROJECT_NAME} PUBLIC
			"${CMAKE_SOURCE_DIR}/lib/SDL2/libSDL2.dll"
			"${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/libSDL2_mixer.dll"
			opengl32
			winmm
			imm32
			version
			ole32
			gdi32
		)

		target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)
	endif()
endif()


# TODO: Windows support (needs testing)
if(WIN32)
	target_link_libraries(${PROJECT_NAME} PUBLIC
		"${CMAKE_SOURCE_DIR}/lib/SDL2/SDL2.a"
		"${CMAKE_SOURCE_DIR}/lib/SDL2_mixer/SDL2_mixer.a"
		opengl32
		winmm
		imm32
		version
	)

	target_compile_definitions(${PROJECT_NAME} PRIVATE SDL_MAIN_HANDLED)
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
		-MMD -MP
	>
	$<$<CONFIG:Release>:
		# -march=native -> Optimize for current CPU (may not work on all CPUs)
		# -DNDEBUG -> Remove debug symbols
		-O3 -march=native -ffast-math -DNDEBUG
	>
)

