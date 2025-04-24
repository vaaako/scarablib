#pragma once

#include <GL/glew.h>
#include <vector>
#include "scarablib/typedef.hpp"

// OpenGL Element Array Buffer object
class EBO {
	public:
		// Make an EBO using a vector of indices
		template <typename T>
		EBO(const std::vector<T>& indices) noexcept;
		~EBO() noexcept;

		// Disable copy and moving
		EBO(const EBO&) noexcept = delete;
		EBO& operator=(const EBO&) noexcept = delete;
		EBO(EBO&&) noexcept = delete;
		EBO& operator=(EBO&&) noexcept = delete;

		// Activates the EBO in the OpenGL context
		inline void bind() const noexcept {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		}

		// Disabled the EBO in the OpenGL context
		inline void unbind() const noexcept {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

	private:
		GLuint id;
};

template <typename T>
EBO::EBO(const std::vector<T>& indices) noexcept {
	static_assert(
		std::is_same_v<T, uint32> ||
		std::is_same_v<T, uint16> ||
		std::is_same_v<T, uint8>,
		"Only uint32, uint16 and uint8 types for indices are accepted");

	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint32)), indices.data(), GL_STATIC_DRAW);
}
