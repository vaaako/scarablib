#pragma once

#include <GL/glew.h>
#include <vector>
#include "scarablib/typedef.hpp"

// OpenGL Element Array Buffer object
class EBO {
	public:
		// Uses a existing EBO id and does not create a new one
		EBO(const uint32 id) noexcept;

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
		void bind() noexcept {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
		}

		// Disabled the EBO in the OpenGL context
		inline void unbind() noexcept {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

		// Returns the EBO id
		inline uint32 get_id() const noexcept {
			return this->id;
		}

	private:
		uint32 id;
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
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(T)), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
