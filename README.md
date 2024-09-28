# Scarablib
Scarablib 🪲 is a C++ library for graphical development.

Currently Scarablib is in early stages of development. You can see the file [`todo.md`](doc/todo.md), to follow the development. This is my main project but still slowly being developed as an hobby.

See [examples/](examples/) to check some features (not all features have examples yet)

# Features
> [!NOTE]
> Scarablib is in pre-alpha

- Abstraction
- Easy 2D and 3D development with multiple features
- Helpful utility methods included
- All dependencies are bundled within the library

# Libraries
- [`SDL2`](https://www.libsdl.org/) for window management
	+ [`SDL2_mixer`](https://www.libsdl.org/projects/mixer/) for sound
	+ [`SDL2_ttf`](https://wiki.libsdl.org/SDL2_ttf/FrontPage) for fonts
- [`stb`](https://github.com/nothings/stb/tree/master) for image loading
- [`GLEW`](https://glew.sourceforge.net/) for OpenGL
- [`glm`](https://github.com/g-truc/glm) for mathematics
<!-- - [`ENet`](https://github.com/zpl-c/enet) (fork) for networking -->


<!-- # OpenGL folder -->
<!-- a folder with multiple funcionalities for helping with opengl that can be used by itself -->


<!-- # Roadmap -->
<!-- - Load `obj` and `gltf` models -->
<!-- - Light source -->
<!-- - Network support -->

# Building and run
You can use both `CMake` and `Makefile` to build the library and run tests.
- Use **Makefile** for building the library and running tests.
- Use **CMake** primarily for testing.

No futher installation needed

## CMake
You can compile `test/main.cpp` using `CMake`, which is the file used for testing the library
```sh
cmake -S . -B build    # Config
cmake --build build    # Compile
./build/scarablib_dev  # Run
```

## Makefile
Currently with `Make` you can only compile `test/main.cpp`, wich is the file used for testing the library
```sh
make dev              # Compile
./build/scarablib_dev # Run
```

## Scripts
- `compile_and_run.sh`: Uses `make` to compile and run
- `debug.sh`: Debug using valgrind
-  `build_compile_commands.sh`: Uses [bear](https://github.com/rizsotto/Bear) to generate `compile_commands.json` file (Used for LSP)


<!-- # Inspirations -->
<!-- gunslinger -->
<!-- rayllib -->
