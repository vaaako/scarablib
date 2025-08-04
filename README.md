# Scarablib
Scarablib 🪲 is a C++ library designed to simplify graphical development, offering tools for both 2D and 3D rendering. While still in its early stages, Scarablib is being developed as a hobby project. You can track its progress in the [`todo.md`](doc/todo.md) file.

![showcase](medias/showcase.gif)

# Features
> [!NOTE]
> Scarablib is in pre-alpha

- **Abstraction:** Simplifies complex graphical operations
- **2D and 3D development:** Easy-to-use tools for both 2D and 3D rendering
	+ Multiple built-in 2D and 3D shapes
	+ Easy texture loading and management
- **Beginner-Friendly API:** Designed with simplicity in mind, it is easy to learn for new developers while remaining powerful enough for advanced users
- **Uniform Grid:** Built-in spatial partitioning system for collision detection
- **Simplified VAO creation:** Dynamic methods for creating Vertex Array Objects with minimal boilerplate

# Libraries used
- [`SDL2`](https://www.libsdl.org/) for window management
	+ [`SDL2_mixer`](https://www.libsdl.org/projects/mixer/) for sound handling
- [`stb_image`](https://github.com/nothings/stb) for image loading
- [`stb_truetype`](https://github.com/nothings/stb) for TTF loading
- [`glad`](https://github.com/Dav1dde/glad) for OpenGL support
- [`glm`](https://github.com/g-truc/glm) for vector and matrix math
- [`tinyobjloader`](https://github.com/tinyobjloader/tinyobjloader) for wavefront obj loader

>You can find the licenses for these libraries in the [`licenses/`](licenses/) directory

# Roadmap
- **Physics and collisions:** In progress
- **Light:** Implementation of light sources
- **Networking:** Adding network support for multiplayer or distributed applications

# Documentation
Currently there is no documentation. But each method is documented in the source code as a comment above it. If you are using a LSP you can easily view it

# Example
This example creates a 3D scene with a rotating cube
```cpp
#include <scarablib/window/window.hpp>
#include <scarablib/input/keycode.hpp>

int main() {
	// Window setup
	Window window = Window({
		.width = 800,
		.height = 600,
		.title = "Hello World"
	});

	// Scene3D Setup
	Scene3D scene3d = Scene3D(camera);

	// Model setup
	Cube* cube = scene3d.add<Cube>("cube"); // Add to scene for batch drawing
	cube->set_position({ 0.0f, 0.0f, -5.0f });
	cube->set_color(Colors::MAGENTA);
	cube->material.texture = new Texture("textures/cube.png");

	// Used to rotate the cube
	float angle = 0.0f;

	// Main loop
	while(window.is_open()) {
		// Window stuff
		window.clear();
		window.process_events();

		// Handle events
		if(window.has_event(Event::WINDOW_QUIT)) {
			window.close();
		}

		// Change cube's angle
		//                angle    axis (x, y, z)
		cube->set_rotation(angle, { true, true, false });
		// Draw all models added to the scene with batch drawing
		scene.draw_all();

		// Change cube's angle
		angle += 1.0f;
		if(angle >= 360.0f) {
			angle = 0.0f;
		}

		// Swap buffers
		window.swap_buffers();
	}
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

Installing this library is done by running
```sh
sudo make install
```

You can then compile your own project linking the library and OpenGL. Example:
```sh
clang++ main.cpp -lGL -lscarablib -o mygame
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

