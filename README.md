# Scarablib
Scarablib 🪲 is a C++ library designed to simplify graphical development, offering tools for both 2D and 3D rendering. While still in its early stages, Scarablib is being developed as a hobby project. You can track its progress in the [`todo.md`](docs/todo.md) file.

![showcase](medias/showcase.gif)

# Features
> [!NOTE]
> Scarablib is in pre-alpha

- **Beginner-Friendly API**: Designed with simplicity in mind, it is easy to learn for new developers while remaining powerful enough for advanced users
- **2D and 3D development**: Easy-to-use tools for both 2D and 3D rendering
	+ 2D and 3D basic shapes, Billboard and multiple type of planes
	+ Material Component with Color, Shader and Texture
- **Memory Management**: The memory is exclusively managed by the library
	+ The library also contains a built-in resource manager and batch drawing
- **Simplified VAO creation**: Dynamic methods for creating Vertex Array Objects with minimal boilerplate
- **Built-in systems**: Raycasting, Terrain Map (collision for 2D images), Uniform Grid (collision for custom 3D models)
- **Custom Shader Support**: Support for creation of customizable shaders

# Libraries used
- [`SDL2`](https://www.libsdl.org/) for window management
	+ [`SDL2_mixer`](https://www.libsdl.org/projects/mixer/) for sound handling
- [`stb_image`](https://github.com/nothings/stb) for image loading
- [`stb_truetype`](https://github.com/nothings/stb) for TTF loading
- [`glad`](https://github.com/Dav1dde/glad) for OpenGL support
- [`glm`](https://github.com/g-truc/glm) for vector and matrix math
- [`tinyobjloader`](https://github.com/tinyobjloader/tinyobjloader) for wavefront obj loader

>You can find the licenses for these libraries in the [`licenses/`](licenses/) directory

## Dependencies
- [glad header](include/external/glad/gl.h)

# Roadmap
- **Physics and collisions:** In progress
- **Light:** Implementation of light sources
- **Networking:** Adding network support for multiplayer or distributed applications

# Documentation
Currently there is no documentation. But each method is documented in the source code as a comment above it. If you are using a LSP you can easily 

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
# Macros
These are all the macros you can enable when compiling the library

## Funcionality
- `BUILD_OPGL30`: Use `OpenGL 3.0` compatible methods instead of `OpenGL 4.5` DSA methods (default)

## Debug
- `SCARAB_DEBUG_VAO_MANAGER`: Shows information of `VertexArray` creation
- `SCARAB_DEBUG_SHADER_MANAGER`: Shows information of `Shader` and `ShaderProgram` creation 
- `SCARAB_DEBUG_DRAW_ALL`: Shows information of `VertexArray` creation

## Static Library (Recommended)
Build the static library:
```sh
cmake -B build -DSTATIC=1 -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## Shared Library
Build the shared library:
```sh
cmake -B build -DSHARED=1 -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

This will not merge `SDL2` and `SDL2_mixer`, you will need to put the shared files in your project and link them

## Debug
If build type is set to **Debug**, CMake will look for the `test/main.cpp` file and build the executable `build/scarablib_test`
```sh
cmake -B build -DSTATIC=1 -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Using
Now you just need to move the headers and the generated library. In order to use the [template Makefile](docs/Makefile) you need to move your `cpp` files to a folder named `src/`, the generated library to a folder named `lib/scarablib/`, `include/scarablib/` and `include/external/glad/` to a folder named `included/`. If done correctly you should be able to use the command `make` and it will compile it with no errors

## Windows Support
Windows support is implemented but not tested. Currently only `SDL2` static library is used, you will have to ship `SDL2_mixer.dll`

# Contributing
Scarablib is a hobby project, and contributions are welcome! If you’d like to contribute, please open an issue or submit a pull request

# LICENSE
This project is licensed under a modified version of the zlib License. See the [LICENSE](LICENSE) file for details

