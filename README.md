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

# Roadmap
- Load `obj` and `gltf` models
- Light source
- Network support

# Example
This scene was made basically with the code below

![example.gif](medias/example.gif)
```cpp
// Draw 3D shapes
scene3d.draw_all({
	&cube1,
	//                  center, angle, radius
	&cube2.set_position(vecutil::orbitate_y(cube1.get_position(), rotation, 2.0f))
});

// Draw 2D shapes
// Draw font
scene2d.draw_shape(
	msgothic.set_text("FPS: " + std::to_string(window.fps()))
);

// Aim
scene2d.draw_shape(rectangle);

// Update rotation
rotation += rotation_speed;
if(rotation >= 360.0f) {
	rotation = 0.0f; // Keep the angle within 0-360 degrees
}
```



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
## Static library
With `Make` you can compile the library to a static library to use on your own project
```
make
```

This will generate a static library under `build/`

>The generated library is kinda big, but this is `SDL_ttf` fault

## Dev
You can can compile `test/main.cpp`, wich is the file used for testing the library
```sh
make dev              # Compile
./build/scarablib_dev # Run
```

# Windows Support
Currently the library does not provide a windows support, but you can try to compile it yourself

## Scripts
- `compile_and_run.sh`: Uses `make` to compile and run
- `debug.sh`: Debug using valgrind
- `build_compile_commands.sh`: Uses [bear](https://github.com/rizsotto/Bear) to generate `compile_commands.json` file (Used for LSP)


<!-- # Inspirations -->
<!-- gunslinger -->
<!-- rayllib -->
