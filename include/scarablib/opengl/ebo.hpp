#pragma once

#include <GL/glew.h>
#include <vector>
#include "scarablib/typedef.hpp"

// OpenGL Element Array Buffer wrapped object.
// This class handles the creation and management of an Element Buffer Object (EBO),
// which is used to specify indices for drawing elements in OpenGL.
class EBO {
	public:
		// Make an EBO using a vector of indices
		EBO(const std::vector<uint32>& indices);
		EBO(const std::vector<uint16>& indices);
		~EBO();

		// Disable copy and moving
		EBO(const EBO&) = delete;
		EBO& operator=(const EBO&) = delete;
		EBO(EBO&&) = delete;
		EBO& operator=(EBO&&) = delete;

		// This effectively disables the EBO
		inline void unbind() {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	private:
		GLuint id;
};

