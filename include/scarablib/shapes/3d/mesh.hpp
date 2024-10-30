#pragma once

#include <GL/glew.h>
#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/scenes/camera.hpp"
#include "scarablib/typedef.hpp"

// An object used for drawing 3D shapes
struct Mesh {
	Mesh(const VAO* vao, const uint32 indices_length = 0);
	// Mesh(const char* path); // For obj files

	// Init mesh using a vector of Vertex and Indices
	// Mesh(const std::vector<Vertex>& data, const std::vector<uint32>& indices);
	// Init mesh using a vector of Vertices, Texture Coordinates (optional) and Indices
	// Mesh(const std::vector<float>& vertices, const std::vector<float>& tex_coords, const std::vector<uint32>& indices);

	const VAO* vao = nullptr; // Reference to some VAO
	uint32 indices_length; // Not const because on obj loading constructor, is not possible to set on runtime

	// Draw current mesh.
	// It needs a camera object and a shader
	virtual void draw(const Camera& camera, const Shader& shader) = 0;

	// Used for derived meshes
	virtual inline VAO& get_vao() = 0;
};

