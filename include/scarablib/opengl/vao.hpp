#pragma once

#include <GL/glew.h>

// OpenGL Vertex Array Object wrapped class.
// This class manages a Vertex Array Object (VAO), which stores the configuration 
// of vertex attribute pointers and the associated VBOs.
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

		// Activates the VAO in the OpenGL context.
		// This enables the configuration stored in the VAO.
		inline void bind() const noexcept {
			glBindVertexArray(this->id);
		}

		// This effectively disables the VAO
		inline void unbind() const noexcept {
			glBindVertexArray(0);
		}

		inline GLuint get_id() const noexcept {
			return id;
		}

	private:
		GLuint id;
};
