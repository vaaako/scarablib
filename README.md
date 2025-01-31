# Scarablib
Scarablib 🪲 is a C++ library designed to simplify graphical development, offering tools for both 2D and 3D rendering. While still in its early stages, Scarablib is actively being developed as a hobby project. You can track its progress in the [`todo.md`](doc/todo.md) file.

<!-- See [examples/](examples/) to check some features (**note:** not all features have examples yet) -->

![showcase](medias/showcase.gif)

# Features
> [!NOTE]
> Scarablib is in pre-alpha

- **Abstraction:** Simplifies complex graphical operations
- **2D and 3D development:** Easy-to-use tools for both 2D and 3D rendering
- **Built-in Optimization:** Methods like frustum-culling for improved performance.

# Libraries used
- [`SDL2`](https://www.libsdl.org/) for window management
	+ [`SDL2_mixer`](https://www.libsdl.org/projects/mixer/) for sound handling
- [`Freetype`](http://freetype.org/) for font loading
- [`stb`](https://github.com/nothings/stb/tree/master) for image loading
- [`GLEW`](https://glew.sourceforge.net/) for OpenGL support
- [`glm`](https://github.com/g-truc/glm) for mathematical operations

>You can find the licenses for these libraries in the [`licenses/`](licenses/) directory

# Roadmap
- **Model loading:** Support for `obj` (alredy supported) and `gltf` models
- **Lighting:** Implementation of light sources
- **Networking:** Adding network support for multiplayer or distributed applications
- **Optimization builtin:** Optimization methods like frustum-culling builtin

# Example
The following code snippet demonstrates how to create a simple scene with Scarablib:

![example.gif](medias/example.gif)

Outside main loop:
```cpp
Cube* cube = ModelFactory::create_cube({
	// This is a required arg (will change later so can set it to 0.0)
	.position = vec3<float>(0.0f),
});

// Above is the same as passing nothing
Cube* cube2 = ModelFactory::create_cube({});

// Set textures
cube1->set_texture(&tex1);
cube2->set_texture(&tex2);

// Add to the scene
scene3d.add_to_scene("cube1", cube); // Models added to scene must be pointers (this will be automatically deleted)
scene3d.add_to_scene("cube2", cube2);

// Rectangle object
Rectangle* aim = new Rectangle({
	.position = vec2<uint32>(
		window.get_half_width()  - 5,
		window.get_half_height() - 5
	),
	.size = vec2<float>(10.0f, 10.0f)
});
scene2d.add_to_scene("aim", aim);
```

Inside the main loop:
```cpp
// Modify cube
//                                        center,               angle, radius
cube2.set_position(ScarabMath::orbitate_y(cube1.get_position(), rotation, 2.0f));
// Draw 3D shapes
scene3d.draw_all();

// Draw font
msgothic.draw_text("FPS: " + std::to_string(window.fps()), { 0.0f, 0.0f });
// Draw 2D shapes
scene2d.draw_all();

// Update rotation
rotation += rotation_speed;
if(rotation >= 360.0f) {
	rotation = 0.0f; // Keep the angle within 0-360 degrees
}
```



# Building and run
Scarablib can be built using either `CMake` or `Makefile`. Both methods are supported, but they serve slightly different purposes:

## CMake
Use CMake for testing the library:
```sh
cmake -S . -B build    # Configure
cmake --build build    # Compile
./build/scarablib_dev  # Run
```

This will compile the file `test/main.cpp`, which is the file used for testing the library

## Makefile
With `Make` you can compile the library to a shared and static library to use on your own projects:
```
make
```

This will generate a shared library in the `build/` directory

For static library, use:
```
make static
```

## Development
To compile and run the test file (`test/main.cpp`):
```sh
make dev              # Compile
./build/scarablib_dev # Run
```

Unlike CMake, this will compile the library as a shared file and then link to it

# Windows Support
Currently, Scarablib does not officially support Windows. However, you can attempt to compile it yourself

## Scripts
<!-- - `compile_and_run.sh`: Uses `make` to compile and run -->
- `debug.sh`: Debug using valgrind
- `build_compile_commands.sh`: Uses [bear](https://github.com/rizsotto/Bear) to generate `compile_commands.json` file for LSP integration


<!-- # Inspirations -->
<!-- gunslinger -->
<!-- rayllib -->
