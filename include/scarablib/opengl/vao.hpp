#pragma once

#include <GL/glew.h>

// OpenGL Vertex Array Object object
class VAO {
	public:
		// Creates a VAO and generates an OpenGL buffer ID
		VAO() noexcept;
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
		inline GLuint get_id() const noexcept {
			return id;
		}

	private:
		GLuint id;
};
