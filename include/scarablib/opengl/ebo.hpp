#pragma once

#include <GL/glew.h>
#include <vector>
#include "scarablib/typedef.hpp"

// OpenGL Element Array Buffer wrapped object.
// This class handles the creation and management of an Element Buffer Object (EBO),
// which is used to specify indices for drawing elements in OpenGL.
class EBO {
	public:
		// Make an EBO using a vector of indices.
		// Recommended types: uint32 and uint16
		template <typename T = uint32>
		EBO(const std::vector<T>& indices);
		~EBO();

		// This effectively disables the EBO
		inline void unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	private:
		GLuint id;
};

// Template must be declared and implemented in the same file
template <typename T>
EBO::EBO(const std::vector<T>& indices) {
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(T), indices.data(), GL_STATIC_DRAW);
}
