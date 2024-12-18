#pragma once

#include <GL/glew.h>

// OpenGL Vertex Array Object wrapped class.
// This class manages a Vertex Array Object (VAO), which stores the configuration 
// of vertex attribute pointers and the associated VBOs.
class VAO {
	public:
		// Creates a VAO and generates an OpenGL buffer ID
		VAO();
		~VAO();

		// Disable copy and moving
		VAO(const VAO&) = delete;
		VAO& operator=(const VAO&) = delete;
		VAO(VAO&&) = delete;
		VAO& operator=(VAO&&) = delete;

		// Activates the VAO in the OpenGL context.
		// This enables the configuration stored in the VAO.
		inline void bind() const {
			glBindVertexArray(this->id);
		}

		// This effectively disables the VAO
		inline void unbind() const {
			glBindVertexArray(0);
		}

		inline GLuint get_id() const {
			return id;
		}

		// This effectively disables the VAO
		static inline void unbind(const GLuint id) {
			glBindVertexArray(id);
		}

		// Activates the VAO in the OpenGL context using a existing ID.
		// This enables the configuration stored in the VAO.
		static inline void bind(const GLuint id) {
			glBindVertexArray(id);
		}

	private:
		GLuint id;
};
