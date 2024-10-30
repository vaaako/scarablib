# Note
I wrote most of the things here very tired, sorry for any english mistake

# TODO library
- [x] Use my vectores instead of glm
	- matrix still glm ones tho
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
- [ ] Discord RPC support

# TODO Bug
- [ ] Memory leak somewhere (i don't know if is in my code or in some library, but is not dangerous)
- [X] Fix FPS drop when movement is enabled

# TODO Window
- [ ] Window Events
	+ [X] Custom event enums
	+ [X] Multiple events support
	+ [ ] Resizing support (Is added but need to change viewport)
	+ [ ] Mouse handle clicks and motion like keyboard
- [x] Custom keyboard enums

# TODO Features
- [ ] More texture options
	+ [ ] Different texture for each face
	+ [ ] Texture overlay
	+ [ ] Texture atlas support
	+ [ ] Opacity
- [x] Batch rendering
- [x] Use the same font object for different texts

# TODO Shapes
- [ ] Custom shapes support
- [ ] Color matrix support

# TODO 2D
- [x] Unify shader and `shader_texture` somehow
	+ Use a blank texture as default (texture made in code)
	+ Add to vertex attrib 2 on shape VBO and check if is to use on shape instance
	+ This could be done by making a custom shape (e.g. `new Cube(custom_color)`), then all variants of this shape with custom color would have this color

# TODO 3D
- [x] Camera
	+ [X] **[DISCARTED]** [Camera Movement](https://github.com/vaaako/Vakraft/blob/main/src/main/java/com/magenta/main/Game.java#L121)
	+ [X] Best mouse movement
	+ [X] Custom position for shapes
	+ [X] Rotation of 3D shape not working
	+ [X] Shapes custom scale
	+ [ ] Review [camera movement](https://github.com/swr06/Minecraft/blob/master/Source/Core/Camera.cpp)
- [ ] Chunk mesh native support
- [ ] Merge `set_angle` and `set_axis` somehow
- [ ] Sphere
- [ ] Skybox
- [ ] Normals / Shading value (both?)


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
- [x] opengl/
	+ [x] shaders/
		* [x] 2d/
		* [x] 3d/
	+ [x] ebo.hpp
	+ [x] shader_program.hpp
	+ [x] vao.hpp
	+ [x] vbo.hpp
- [x] proper/
	+ [x] vector/
		* [x] vec2.hpp
		* [x] vec3.hpp
		* [x] ...
	+ [x] color.hpp
- [x] scenes/
	+ [x] scene.hpp
	+ [x] scene2d.hpp
- [x] shapes/
	+ [x] shape2d.hpp
- [x] types/
	+ [x] audio.hpp
	+ [x] color.hpp
	+ [x] font.hpp
	+ [x] music.hpp (may contain some bug)
	+ [x] sound.hpp
	+ [x] texture.hpp (finish when implement fonts)
	+ [x] vertex.hpp
- [x] utils/
	+ [x] file.hpp
	+ [x] log.hpp
	+ [x] math.hpp
	+ [x] sdl.hpp
	+ [x] string.hpp
- [x] window/
	+ [x] events.hpp
	+ [x] window.hpp
+ [x] typedef.hpp
