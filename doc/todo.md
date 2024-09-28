# Target
- [X] Use Vector2f and Vector3f instead of glm versions
- [X] Custom keyboard enums
- [X] Structs to `.cpp`
- [X] BUG: If font is not a pointer, the window has a error when trying to close and the program has a segfaut
	+ Maybe it has something to do with the texture pointer in `font.hpp`
	+ **SOLUTION:** I was closing `TTF_Font` when i shoudn't, in `font.cpp`
- [X] **[DISCARTED]** Add `template struct vec<>` for vectors
- [X] Split `shapes/` and `renderers/`
	+ I don't know if `renderes/` is the correct name
- [X] View `Raylib` source code
- [X] Fix log `fmt`
	+ [X] Log for `audio.cpp` and `font.cpp` -> instead of logging full path, make a method on `StringHelper` to get only the file name
- [X] Avoid circular import for `set_cursor_position` in `window.hpp`
- [X] **[!]** Move draw to the shape struct. e.g. Cube -> import cube shape -> `cube.draw(args)`;
	+ [X] Rename `shape.hpp` to `mesh.hpp`
	+ [X] Renderer3D -> `renderer.draw_cube(texture)` -> `cube.draw(texture)`
	+ [Tutorial](https://www.youtube.com/watch?v=NUZF_5RKfS4)

- [ ] Change library name
	+ Beetle relatable
	+ Coleoptera
	+ Scarablib?
	+ Staglib
- [X] Camera
	+ [X] **[DISCARTED]** [Camera Movement](https://github.com/vaaako/Vakraft/blob/main/src/main/java/com/magenta/main/Game.java#L121)
	+ [X] Best mouse movement
	+ [X] Custom position for shapes
	+ [X] Rotation of 3D shape not working
	+ [X] Shapes custom scale
	+ [ ] Review [camera movement](https://github.com/swr06/Minecraft/blob/master/Source/Core/Camera.cpp)
- [ ] Unify shader and `shader_texture` somehow
	+ I don't think this is possible
	+ Maybe set the texture alpha to zero when using just solid color?
	+ If not, put the if-check on glsl file
- [ ] More texture options
	+ [ ] Different texture for each face
	+ [ ] Texture overlay
	+ [ ] Texture atlas support
	+ [ ] Opacity
- [ ] Window Events
	+ [X] Custom event enums
	+ [X] Multiple events support
	+ [ ] Resizing support. Is added but need to change viewport
	+ [ ] Mouse handle click and motion at the same time
	+ [ ] More window events
- [ ] Memory leak somewhere (i don't know if is on my `font.hpp` or SDL2 one)
- [ ] Put required libraries `shared libraries` files on `libs/` folder
	+ [ ] Try again to merge all static libraries on the Dynastinae library
- [ ] Assets path relative to file instead of executable
- [ ] Fix FPS drop when movement is enabled
- Shapes
	+ [ ] Merge `set_angle` and `set_axis`
	+ [ ] Convert Cube and Pyramid vertex to `0.5f`
	+ [ ] [Useful for add](https://www.youtube.com/watch?v=5URiEpC5PGw)
	+ [ ] Manipulate faces
	+ [ ] Sphere
	+ [ ] Support for drawing multiple meshes as one big mesh. Like [here](https://github.com/vaaako/Vakraft/blob/main/src/main/java/com/magenta/game/Chunk.java)
	+ [ ] Skybox
	+ [ ] Normals / Shading value (both?)
- [ ] lib folder instead of downloading lib
	+ [ ] Edit Makefile to use the libraries of `libs/` and header of `include/` instead of in the files in local machine
- [ ] Replace `enum class` to `enum` in some cases where there are multiple conversions
- [ ] All graphics logic possible to glsl *(GPU)*

# Notes
- **[?]** Custom vertex colors for 2D
	+ Add to vertex attrib 2 on shape VBO and check if is to use on shape instance
	+ This could be done by making a custom shape (e.g. `new Cube(custom_color)`), then all variants from this shape with custom color would have this color
- [X] When creating a `Object` -> `Model obj = Model("assets/models/character.obj")`
	+ `Model` will be a struct with `VAO` and `VBO` *(possibly a child of `Shape`)*
	+ `camera.draw_obj(obj)`

# Features
- Sprite atlas
- Tick
- Entity manager?

## 3D features
- Collision
- Level of Detail (LOD)
- Frustum Culling

## 2D features
- Collision
- Follow camera
- Load tile (tiled)

# Review files
Review for erros and improvement, in both header and declaration

- [ ] input/
	+ [x] keyboard.hpp
	+ [x] keycode.hpp
	+ [ ] mouse.hpp (recheck when implementing 3D space)
- [ ] opengl/
	+ [x] shaders/
		* [x] 2d/
		* [x] 3d/
	+ [ ] ebo.hpp
	+ [x] shader_program.hpp
	+ [x] vao.hpp
	+ [ ] vbo.hpp
- [x] proper/
	+ [x] vector/
		* [x] vec2.hpp
		* [x] vec3.hpp
		* [x] ...
	+ [x] color.hpp
- [ ] scenes/
	+ [ ] scene.hpp
	+ [ ] scene2d.hpp
- [ ] shapes/
	+ [ ] shape2d.hpp
- [ ] types/
	+ [x] audio.hpp
	+ [x] color.hpp
	+ [ ] font.hpp
	+ [x] music.hpp (may contain some bug)
	+ [x] sound.hpp
	+ [ ] texture.hpp
	+ [ ] vertex.hpp
- [ ] utils/
	+ [ ] file.hpp
	+ [ ] log.hpp
	+ [ ] math.hpp
	+ [ ] sdl.hpp
	+ [x] string.hpp
- [ ] window/
	+ [x] events.hpp
	+ [ ] window.hpp
+ [x] typedef.hpp
