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
		EBO(const std::vector<uint32>& indices) noexcept;
		EBO(const std::vector<uint16>& indices) noexcept;
		~EBO() noexcept;

		// Disable copy and moving
		EBO(const EBO&) noexcept = delete;
		EBO& operator=(const EBO&) noexcept = delete;
		EBO(EBO&&) noexcept = delete;
		EBO& operator=(EBO&&) noexcept = delete;

		inline void bind() const noexcept {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		}

		// This effectively disables the EBO
		inline void unbind() const noexcept {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	private:
		GLuint id;
};

