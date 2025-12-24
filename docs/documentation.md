# UNDER CONSTRUCTION
# I HAVE TO WRITE THIS

# About
This is the documentation for my game library scarablib. This contains the main features of the main classes

This documentation does not content all single methods avaible. Every method has a description in code of its behaviour above.

# Content
1. [Window](#window)
	+ 1.1. [Events](#events)
	+ 1.2. [FPS and DT](#fps-and-dt)
	+ 1.3. [Keyboard handler](#keyboard-handler)
	+ 1.4. [Mouse handler](#mouse-handler)
2. [Shapes](#shapes)
	+ 2.1 [Model](#model)
	+ 2.2 [2D Shapes](#2d-shapes)
	+ 2.3 [3D Shapes](#3d-shapes)
3. [Camera](#camera)
4. [Scenes](#scenes)
5. [Texture](#texture)
6. [Skybox](#skybox)
6. [TerrainMap](#terrainmap)
7. [Text](#text)
8. [Legacy Opengl](#legacy-opengl)


# Window
## Lifetime
The window object must be relased after all other OpenGL objects, otherwhise it will cause a memory leak. If you are uncertain of which objects are OpenGL objects, just release the window object after all others in your application
## Members
## Events
## FPS and DT
## Keyboard handler
## Mouse handler

# Shapes
## Mesh
## Model
## 2D Shapes
## 3D Shapes

# Camera

# Scenes

# Texture

# Timer
## Clock
## Interval
## Alarms



# Custom Shaders

Avaiable uniforms are:
- `shapeColor`: Mesh's color
- `texSampler`: Texture 2D (one pixel solid white if none is set)
- `texSamplerArray`: Texture Array
- `texLayer`: Texture Array's index
- `mixAmount`: mix amount of **texSampler** and **texSamplerArray**

`texSamplerArray`, `texLayer` and `mixAmount` will only have value if a Texture Array is used

If you want to mix `texSampler` and `texSamplerArray` you can use the following snippet
```glsl
vec4 mixedtex = mix(
	// Texture Sampler
	texture(texSampler, texuv),
	// Texture Array Sampler
	texture(texSamplerArray, vec3(texuv, texlayer)),
	// Mix amount
	mixamount
);
```

# Skybox

# TerrainMap

# Raycast

# Text

# OpenGL objects
I recommend under all cases always use `VertexBufferComponent` object over `VAO`, `VBO`, `EBO` and `VAOManager` objects, since `VertexBufferComponent` wrap all these in simple and reliable methods with checks and automation 

You don't need to bind VAO if you intent to use VBO modern methods to set up VAO

Only the modern VAO methods and making is supported (in `VertexBufferComponent` and `Scene3D`)
(i need to change this setence later)
