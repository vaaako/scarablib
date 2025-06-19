#pragma once

#include "scarablib/typedef.hpp"
#include <GL/glew.h>

// OpenGL Vertex Array Object object
class VAO {
	public:
		// Creates a VAO and generates an OpenGL buffer ID
		VAO(const uint32 id = 0) noexcept;
		~VAO() noexcept;

		// Disable copy and moving
		VAO(const VAO&) noexcept = delete;
		VAO& operator=(const VAO&) noexcept = delete;
		VAO(VAO&&) noexcept = delete;
		VAO& operator=(VAO&&) noexcept = delete;

		// Activates the VAO in the OpenGL context
		inline void bind() const noexcept {
			glBindVertexArray(this->id);
		}

		// Disabled the VAO in the OpenGL context
		inline void unbind() const noexcept {
			glBindVertexArray(0);
		}

		// Returns the VAO id
		inline uint32 get_id() const noexcept {
			return this->id;
		}

	private:
		uint32 id;
};
