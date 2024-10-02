#pragma once

#include <GL/glew.h>
#include "scarablib/typedef.hpp"

// OpenGL Vertex Buffer Object wrapped class.
// This class handles the creation, binding, and data management of a Vertex Buffer Object (VBO),
// which stores vertex data on the GPU for rendering.
class VBO {
	public:
		// Initializes a VBO and generates an OpenGL buffer ID
		VBO();
		~VBO();

		// Disable copy and moving
		VBO(const VBO&) = delete;
		VBO(VBO&&) = delete;
		VBO& operator=(const VBO&) = delete;
		VBO& operator=(VBO&&) = delete;

		// Activates the VBO in the OpenGL context
		inline void bind() const {
			glBindBuffer(GL_ARRAY_BUFFER, this->id);
		}

		// This effectively disables the VBO
		inline void unbind() const {
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		// Allocate and initialize the data store for the VBO.
		// `size` is the total size of the data (in bytes) to allocate, and `data` is a pointer to the data.
		// If `data` is nullptr, the buffer is allocated but left uninitialized.
		void alloc_data(const uint64 size, const void* data, const GLenum drawtype = GL_STATIC_DRAW);

		// Link a vertex attribute to the VBO for use in a shader.
		// `index` is the index of the vertex attribute in vertex shader. (e.g, "`layout (location = index) in ...`")
		// `dimension` is the number of components per vertex attribute (e.g., 3 for vec3, 2 for vec2).
		// `total_byte_size` is the total size of a single vertex (e.g., sum of all dimensions times `sizeof(flaot)`).
		// `offset` is the byte offset of the attribute in a vertex (e.g., texture offset of a 3D mesh is is `3 * sizeof(float)`, since position has 3 float values and texture comes after it)
		void link_attrib(const uint32 index, const uint32 dimension, const int total_byte_size, const uint32 offset);

	private:
		GLuint id;
};
