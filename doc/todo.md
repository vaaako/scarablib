# Note
I wrote most of the things here very tired, sorry for any english mistake

# Need to test until it works
- Bounding box

# Currently working on
Stuff that are in progress and i need to finish

- Legacy Mode
- Timer
- Fonts
- Texture Array
- Bounding Box debug draw
- Collision
- Physics
- Terrain Map
- Network

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

- [x] All graphics logic possible to glsl *(GPU)*
- [x] Replace `enum class` to `enum` in some cases where there are multiple conversions
- [x] Mouse "GRABBED" enum (or similar)
- [x] Shader in billboard and circle will make a new one for each instance, make static
- [x] Optimize scene different shader bind by changing the local shader when different then changing back

- [x] Move `Scene3D::add_to_scene` to interface IScene
- [x] Shader from Scene3D and Scene2D use shader manager

- [x] Having problems with shaders, when compiling to executable
	+ not happy with the solution, since it stores shaders forever in memory
- [x] Use bitwise flags when fits
	+ Textures
	+ Billboard
	+ Window?
- [x] Use `string_view` instead of `string` when possible (when string manipulation isnt necessary)
- [x] Class members that dont need any treatment or additional code in a setter method, should be public (when this is the case a setter method for this member is not necessary)
- [x] Change methods description to use "\`\`" for parameters
- [x] Fix forward declarations
- [x] REMOVE (almost) ALL INLINES

- [x] Better Material Component and texture handling
- [x] TextureHandle accept temporary texture object. For this to work change ref to shared_ptr
- [ ] In Window::rotate i didnt have to clear the rotation before, why do i need it now? ([last commit]( https://github.com/vaaako/scarablib/commit/d2a5717958a1f73fde39dafbd6640e2c19dab076))
- [ ] make transparency work without `if(tex.a == 0.0)`?
- [ ] Discord RPC support?

- [ ] Putt built in not related engine classes in a `builtin` folder?
	+ UniformGrid
	+ Raycast
	+ TerrainMap
	+ etc

- [ ] Revise ShaderManager
	+ I dont know if its fully optimized and working properly

# TODO Quick Fixes
- [x] Better texture constructor
	+ Use bitwise paramters
	+ Paramter to choose between RGB and RGBA manually (check inside if image is really RGBA)
	+ Organize constructors
	+ Feature: change texture alpha

- [x] Simplify Keyboard and Mouse input and put all that in Window class
- [x] Unify bundle and VAOManger somehow idk get rid of bundle

# TODO Network
- [ ] Send message when client is disconnected
- [ ] Remove `result` from Network class

# TODO Bug
- [x] Memory leak somewhere (i don't know if is in my code or in some library, but is not dangerous)
	+ It appears to be related to SDL2 and its dependencies (X11, PulseAudio, etc.) 
- [X] Fix FPS drop when movement is enabled
- [ ] Circle alpha may not be working

# TODO Window
- [x] Window Events
	+ [X] Custom event enums
	+ [X] Multiple events support
	+ [x] Resizing support (Is added but need to change viewport)
	+ [x] Mouse handle clicks and motion like keyboard
- [x] Custom keyboard enums

# TODO Features
- [ ] More texture options
	+ [ ] Texture overlay
	+ [ ] Texture atlas support
		* For reference: [anim8](https://github.com/kikito/anim8)
	+ [ ] Opacity
	+ [x] Texture Array (not really used by anything currently)
- [x] Batch rendering
- [x] Use the same font object for different texts
- [ ] Shader support
	+ Define shader object, methods like `load_from_file` and be able to use this custom shader

# TODO Shapes
- [x] Make a Model Factory instead of static `get vao` and a struct for each model
- [x] Custom shapes support (kinda, i think)
- [x] Use one shader for 2D and 3D shapes
- [ ] Color matrix support
- [x] Batch rendering for textures too
	+ Would have to make another map, its worth it?
- [x] Better Shader support for 2D and 3D shapes
	+ for 3d, yeah
- [x] Remove `ModelConf`
	+ kinda?
- [x] CCW vertices

- [ ] TextureArray
	- [ ] Support for texture array and common texture to the same model
	- [ ] make member of "current texture array index" and use it in the shader when drawing a model that uses a texture_array
- [x] Make a new static struct called `ModelFields` that returns `Vertex`'s fields to `ModelFactory` (and the user can use that for easy manipulation)
	+ Like in vakraft, make a helper method for easy change faces values
- [x] Be able to change `texid` and `shading` fields in `ModelFactory` for each face

- [x] Bounding box was not tested
	+ World calculation might not be working correctly (debug drawing not drawing when dynamic transform is enabled)
	+ Not work fully as i want
	+ Not tested
	+ Should also work for 2D shapes and be built in Mesh constructor that uses vertices only
	+ Skipping this because i have more important stuff to do

# TODO 2D
- [x] Unify shader and `shader_texture` somehow
	+ Use a blank texture as default (texture made in code)
	+ Add to vertex attrib 2 on shape VBO and check if is to use on shape instance
	+ This could be done by making a custom shape (e.g. `new Cube(custom_color)`), then all variants of this shape with custom color would have this color
- [x] Change obsolete VAO and VBO management
- [x] Better draw
- [x] Better custom shape support
- [x] Review old shapes
- [ ] New shapes: Triangle (for real)
- [ ] Animation support
	+ For reference: [raycast](https://github.com/vaaako/Raycast/blob/main/src/sprite.lua#L46)
- [ ] Collider class
	+ Like [Windfield](https://github.com/a327ex/windfield). Show collider and easy setting
	+ Rigid body
- [x] If possible, use mesh.hpp for 2D shapes
- [x] Remove `SCARAB_2D_AND_3D`
- [x] Make it work with CULL_FACE
- [ ] Finish font

# TODO 3D
- [ ] Camera
	+ [X] **[DISCARTED]** [Camera Movement](https://github.com/vaaako/Vakraft/blob/main/src/main/java/com/magenta/main/Game.java#L121)
	+ [X] Best mouse movement
	+ [X] Custom position for shapes
	+ [X] Rotation of 3D shape not working
	+ [X] Shapes custom scale
	+ [x] Review [camera movement](https://github.com/swr06/Minecraft/blob/master/Source/Core/Camera.cpp)
	+ [ ] Third person, Front view and top down
	+ Camera movement is stolen from minecraft
- [ ] Chunk mesh native support
- [x] Merge `set_angle` and `set_axis` somehow
- [ ] Sphere
- [ ] Pyramid
- [x] Skybox
- [ ] Normals / Shading value (both?)
- [x] Model and Mesh
- [x] Better solution to AABB
- [x] Camera optimized movement
- [x] Model size is box size
- [x] Camera movement not working properly with VSync disabled
- [x] Make a billboard model
	+ Rotate logic to shader
	+ It will have to be a overrided draw method and other shader
	+ Update model matrix in a method (like 2D shapes)
	+ In scene3d check if model has a shader, if have, use it just for that model
- [x] Don't store vertices, this makes the memory usage huge for each model
	+ fixed, but is much more of a hack than a actual solution
- [x] Billboarding logic to GPU (shaders)
- [x] Billboard: Another approach for changing directional textures? How doom does it? How would a moving billboard work? Changing orientation?
	+ Doom checks for the billboard direction and changes based in it (kinda)
- [x] Billboard: Method to invert rotation
	+ isnt really necessary
- [x] Currently bounding box is always created, make it optional
	+ now needs to calll a method to create it, also the draw is now correct

- [x] Better BoundingBox, not happy with the current state, dont know what to do
- [x] Show bounding box
- [ ] Billboard: Texture array
- [ ] Different texture for each face (the other method that is not cubemap)
- [ ] Ortographic camera
- [x] Model loading to Model class
	+ [ ] Multiple textures handling

- [x] Be able to stand on loaded terrain
	+ Also implement collision with walls
	+ Ray + Grid method?
- [ ] UniformGrid is not fully implemented
	+ Planes are not being detected
	+ Corners are not being detected
	+ Ceilings are not being detected
