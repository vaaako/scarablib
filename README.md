# Scarablib
Scarablib 🪲 is a C++ library designed to simplify graphical development, offering tools for both 2D and 3D rendering. While still in its early stages, Scarablib is being developed as a hobby project. You can track its progress in the [`todo.md`](doc/todo.md) file.

<!-- See [examples/](examples/) to check some features (**note:** not all features have examples yet) -->

![showcase](medias/showcase.gif)

# Features
> [!NOTE]
> Scarablib is in pre-alpha

- **Abstraction:** Simplifies complex graphical operations
- **2D and 3D development:** Easy-to-use tools for both 2D and 3D rendering
- **Beginner-Friendly API:** Designed with simplicity in mind, it is easy to learn for new developers while remaining powerful enough for advanced users
- **Extensible Architecture:** Built to support future enhancements, ensuring scalability for any project type

# Libraries used
- [`SDL2`](https://www.libsdl.org/) for window management
	+ [`SDL2_mixer`](https://www.libsdl.org/projects/mixer/) for sound handling
- [`stb_image`](https://github.com/nothings/stb) for image loading
- [`stb_truetype`](https://github.com/nothings/stb) for TTF loading
- [`GLEW`](https://glew.sourceforge.net/) for OpenGL support
- [`glm`](https://github.com/g-truc/glm) for vector and matrix math
- [`tinyobjloader`](https://github.com/tinyobjloader/tinyobjloader) for wavefront obj loader

>You can find the licenses for these libraries in the [`licenses/`](licenses/) directory

# Roadmap
- **Lighting:** Implementation of light sources
- **Networking:** Adding network support for multiplayer or distributed applications
- **Built-in Optimization:** Methods for frustum-culling, terrain generation and etc, for improved performance.

# Example
The following code snippet demonstrates how to create a simple scene with Scarablib:

![example.gif](medias/example.gif)

Outside main loop:
```cpp
// Position will change later, so dont need to intialize here
Cube* cube = ModelFactory::create_cube();

// Above is the same as passing nothing
Cube* cube2 = ModelFactory::create_cube();

// Set textures
cube1->set_texture(&tex1);
cube2->set_texture(&tex2);

// Add to the scene
scene3d.add_to_scene("cube1", cube); // Models added to scene must be pointers (this will be automatically deleted)
scene3d.add_to_scene("cube2", cube2);

// Rectangle object
const float AIM_WIDTH = 10.0f;
const float AIM_HEIGHT = 10.0f;

Rectangle* aim = SpriteFactory::create_rectangle();
aim->set_position({
	window.get_half_width()  - AIM_WIDTH  / 2,
	window.get_half_height() - AIM_HEIGHT / 2
});
aim->set_size({ AIM_WIDTH, AIM_HEIGHT });

scene2d.add_to_scene("aim", aim);
```

Inside the main loop:
```cpp
// Modify cube
//                                        center,               angle, radius
cube2.set_position(ScarabMath::orbitate_y(cube1.get_position(), rotation, 2.0f));

// Draw all 3D shapes
scene3d.draw_all();

// Draw font
msgothic.draw_text("FPS: " + std::to_string(window.fps()), { 0.0f, 0.0f });

// Draw all 2D shapes
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
CMake in this project is used to compile and run the [test/main.cpp](test/main.cpp) file, which is the file used for testing the library
```sh
cmake -S . -B build    # Configure
cmake --build build    # Compile
./build/scarablib_dev  # Run
```

## Makefile
With `Make` you can compile the library to a shared or static library to use it on your own projects.
The default target will generate a static library in the `build/` directory.

The static target will use the static dependencies libraries located in the `lib/` directory
```
make
```

You can change the target to generate a shared library instead.
Note that for generating the shared library, you don't need to install the dependencies, but for using it, you will need to install the dependencies.

The shared target will use the shared dependencies libraries located in the `lib/` directory.
```
make LIBRARY_TYPE=shared
```


# Windows Support
Currently, Scarablib does not officially support Windows. However, you can attempt to compile it yourself

## Scripts
- `debug.sh`: Debug using valgrind
- `build_compile_commands.sh`: Uses [bear](https://github.com/rizsotto/Bear) to generate `compile_commands.json` file for LSP integration
- `install-flamegraph.sh` and `run-flamegraph.sh`: Easy install and run [flamegraph](https://github.com/brendangregg/FlameGraph)


# Contributing
Scarablib is a hobby project, and contributions are welcome! If you’d like to contribute, please open an issue or submit a pull request

# LICENSE
This project is licensed under a modified version of the zlib License. See the [LICENSE](LICENSE) file for details

