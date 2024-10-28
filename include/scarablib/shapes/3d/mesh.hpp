#pragma once

#include <GL/glew.h>
#include <vector>
#include "scarablib/opengl/shader.hpp"
#include "scarablib/opengl/vao.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/types/vertex.hpp"
 
struct Mesh {
	uint32 indices_length;

	Mesh();
	void init_buffers(const std::vector<float>& vertices, const std::vector<uint32>& indices);
	void init_buffers(const std::vector<float>& vertices, const std::vector<float>& tex_coords, const std::vector<uint32>& indices);
	void init_buffers(const std::vector<Vertex>& data, const std::vector<uint32>& indices);
	virtual ~Mesh();

	// Every mesh must have the same VAO for each clone
	virtual inline VAO& get_vao() = 0;

	void draw(const Camera& camera, const Shader& shader);

	vec3<float> position;
	vec3<float> size = 1.0f;
};
