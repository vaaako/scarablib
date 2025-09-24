#pragma once

#include "scarablib/typedef.hpp"
#include "scarablib/utils/opengl.hpp"
#include <vector>

// WARNING: DO NOT USE DIFFERENT FILES
// USE A MACRO TO DEFINE WHEN TO USE DSA INSIDE vertexarray.hpp
// THE DEFAULT BEING #define USE_VERTEX_ARRAY_DSA (or wathever the name, just enable dsa by default)

// Makes a VAO using DSA.
// Includes methods for making a EBO and VBO using DSA
struct VertexArrayDSA {
	// Pass empty indices if you don't want to make EBO
	template <typename T>
	VertexArrayDSA(const std::vector<T>& indices) noexcept;
	~VertexArrayDSA() noexcept;

	// Links a vertex attribute to the VBO, telling OpenGL how to interpret the vertex data.
	// - `vao_id`: VAO to modify.
	// - `index`: The index of the vertex attribute in the shader (e.g., the 'location' in 'layout(location=index)').
	// - `size`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
	// - `offset`: The offset of this attribute within a vertex.
	// - `type`: Type of the attribute
	// - `binding_index`: (Default: 0) corresponds to a slot in the VAO where a vertex buffer is bound.
	// - `normalize`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1]).
	// normalize only takes effect if type is some floating type
	void link_attrib(const uint32 vao_id, const uint32 index, const uint32 size,
			const GLenum type, const uint32 offset, const uint32 binding_index = 0, const bool normalized = false) const;

	// Links a vertex attribute to the VBO, telling OpenGL how to interpret the vertex data.
	// This method uses Direct State Access (DSA).
	// - `T`: attribute type.
	// - `vao_id`: VAO to modify.
	// - `index`: The index of the vertex attribute in the shader (e.g., the 'location' in 'layout(location=index)').
	// - `size`: The number of components per attribute (e.g., 3 for a vec3, 2 for a vec2).
	// - `offset`: The offset of this attribute within a vertex.
	// - `binding_index`: (Default: 0) corresponds to a slot in the VAO where a vertex buffer is bound.
	// - `normalize`: (Default: false) Whether to normalize the data (e.g., true for [-1, 1] to [0, 1])
	// template <typename T>
	// void link_attrib(const uint32 vao_id, const uint32 index, const uint32 size,
	// 		const uint32 offset, const uint32 binding_index = 0, const bool normalized = false) const noexcept {
	// 	glEnableVertexArrayAttrib(vao_id, index);
	// 	glVertexArrayAttribBinding(vao_id, index, binding_index);
	// 	if constexpr (std::is_integral_v<T>) {
	// 		glVertexArrayAttribIFormat(vao_id, index, size, gl_type<T>(), offset);
	// 	} else {
	// 		glVertexArrayAttribFormat(vao_id, index, size, ScarabOpenGL::gl_type<T>(), normalized ? GL_TRUE : GL_FALSE, offset);
	// 	}
	// }

	private:
		uint32 vao_id;
		uint32 vbo_id;
		uint32 ebo_id;
};

template <typename T>
VertexArrayDSA::VertexArrayDSA(const std::vector<T>& indices) noexcept {
	glCreateVertexArrays(1, &this->vao_id);
	glCreateBuffers(1, &this->vbo_id);
	if(!indices.empty()) {
		glCreateBuffers(1, &this->ebo_id);
		glNamedBufferData(this->ebo_id, sizeof(indices), indices, GL_STATIC_DRAW);
	}
}
