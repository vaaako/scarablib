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
- [x] Use noexcept keyword
- [x] Better color struct

- [ ] All graphics logic possible to glsl *(GPU)*
- [ ] Replace `enum class` to `enum` in some cases where there are multiple conversions
- [ ] Discord RPC support
	+ Currently is very clunky and not dinamic
- [ ] Mouse "GRABBED" enum (or similar)

- [ ] Use `static_assert`
	+ Dont forget about model methods


# TODO Bug
- [x] Memory leak somewhere (i don't know if is in my code or in some library, but is not dangerous)
	+ It appears to be related to SDL2 and its dependencies (X11, PulseAudio, etc.) 
- [X] Fix FPS drop when movement is enabled

# TODO Window
- [ ] Window Events
	+ [X] Custom event enums
	+ [X] Multiple events support
	+ [ ] Resizing support (Is added but need to change viewport)
	+ [x] Mouse handle clicks and motion like keyboard
- [x] Custom keyboard enums

# TODO Features
- [ ] More texture options
	+ [ ] Different texture for each face (the other method that is not cubemap)
	+ [ ] Texture overlay
	+ [ ] Texture atlas support
		* For reference: [anim8](https://github.com/kikito/anim8)
	+ [ ] Opacity
- [ ] Animation support
	+ For reference: [raycast](https://github.com/vaaako/Raycast/blob/main/src/sprite.lua#L46)
- [x] Batch rendering
- [x] Use the same font object for different texts

# TODO Shapes
- [x] Make a Model Factory instead of static `get vao` and a struct for each model
- [x] Custom shapes support (kinda, i think)
- [ ] Color matrix support
- [ ] Use one shader for 2D and 3D shapes
- [ ] Collider class (for 3D and 2D)
	+ Like [Windfield](https://github.com/a327ex/windfield). Show collider and easy setting
	+ Rigid body
- [ ] Physics (dont know how)
- [ ] Batch rendering for textures too
	+ Would have to make another map, its worth it?
- [ ] Better Shader support for 2D and 3D shapes
- [x] Remove `ModelConf`
	+ kinda?

# TODO 2D
- [x] Unify shader and `shader_texture` somehow
	+ Use a blank texture as default (texture made in code)
	+ Add to vertex attrib 2 on shape VBO and check if is to use on shape instance
	+ This could be done by making a custom shape (e.g. `new Cube(custom_color)`), then all variants of this shape with custom color would have this color
- [ ] Change obsolete VAO and VBO management

# TODO 3D
- [x] Camera
	+ [X] **[DISCARTED]** [Camera Movement](https://github.com/vaaako/Vakraft/blob/main/src/main/java/com/magenta/main/Game.java#L121)
	+ [X] Best mouse movement
	+ [X] Custom position for shapes
	+ [X] Rotation of 3D shape not working
	+ [X] Shapes custom scale
	+ [ ] Review [camera movement](https://github.com/swr06/Minecraft/blob/master/Source/Core/Camera.cpp)
- [ ] Billboarding logic to GPU (shaders)
- [ ] Chunk mesh native support
- [x] Merge `set_angle` and `set_axis` somehow
- [ ] Sphere
- [x] Skybox
- [ ] Normals / Shading value (both?)
- [x] Model and Mesh
- [ ] Better solution to AABB
- [x] Camera optimized movement
- [ ] Model size is box size
- [ ] Camera movement not working properly with VSync disabled
- [ ] Currently bounding box is always created, make it optional
- [x] Make a billboard model
	+ Rotate logic to shader
	+ It will have to be a overrided draw method and other shader
	+ Update model matrix in a method (like 2D shapes)
	+ In scene3d check if model has a shader, if have, use it just for that model

- [ ] Don't store vertices, this makes the memory usage huge for each model
- [ ] Billboard: Instead of making a new texture for each face, use texture ID
	+ Use texture id on model too
- [ ] Billboard: Another approach for changing textures? How doom does it? How would a moving billboard work?


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

