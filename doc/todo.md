TODO library
- [ ] Use Vector2f and Vector3f instead of glm versions
- [x] Change library name
	+ Beetle relatable
	+ Coleoptera
	+ Scarablib?
	+ Staglib
- [x] Add required libraries `shared libraries` files on `libs/` folder
	+ [x] Try again to merge all static libraries
	+ [x] Edit Makefile to use the libraries of `libs/` and header of `include/`
- [x] **[DISCARTED]** Assets path relative to file instead of executable

- [ ] All graphics logic possible to glsl *(GPU)*
- [ ] Replace `enum class` to `enum` in some cases where there are multiple conversions

# TODO Bug
- [ ] Memory leak somewhere (i don't know if is on my `font.hpp` or SDL2 one)
- [ ] Fix FPS drop when movement is enabled

# TODO Window
- [ ] Window Events
	+ [X] Custom event enums
	+ [X] Multiple events support
	+ [ ] Resizing support. Is added but need to change viewport
	+ [ ] Mouse handle click and motion at the same time
- [x] Custom keyboard enums

# TODO Features
- [ ] More texture options
	+ [ ] Different texture for each face
	+ [ ] Texture overlay
	+ [ ] Texture atlas support
	+ [ ] Opacity
- [ ] Batch rendering

# TODO 2D
- [ ] Unify shader and `shader_texture` somehow
	+ Maybe set the texture alpha to zero when using just solid color?
	+ put the if-check on glsl file?

# TODO 3D
- [x] Camera
	+ [X] **[DISCARTED]** [Camera Movement](https://github.com/vaaako/Vakraft/blob/main/src/main/java/com/magenta/main/Game.java#L121)
	+ [X] Best mouse movement
	+ [X] Custom position for shapes
	+ [X] Rotation of 3D shape not working
	+ [X] Shapes custom scale
	+ [ ] Review [camera movement](https://github.com/swr06/Minecraft/blob/master/Source/Core/Camera.cpp)
- [ ] Chunk mesh native support
- [ ] Merge `set_angle` and `set_axis`
- [ ] Different texture for each face
- [ ] Sphere
- [ ] Skybox
- [ ] Normals / Shading value (both?)

# Notes
- [ ] Custom vertex colors for 2D
	+ Add to vertex attrib 2 on shape VBO and check if is to use on shape instance
	+ This could be done by making a custom shape (e.g. `new Cube(custom_color)`), then all variants from this shape with custom color would have this color



# Features for later
- Sprite atlas
- Tick
- Entity manager?

## 3D features
- Collision
- Level of Detail (LOD)
- Frustum Culling

## 2D features
- Collision
- Camera
- Load tile map support (Tiled)

# Review files
Review for erros and improvement (header and declaration)

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
