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

		// Activates the VAO in the OpenGL context.
		// This enables the configuration stored in the VAO.
		inline void bind() const {
			glBindVertexArray(this->id);
		}

		// This effectively disables the VAO
		inline void unbind() const {
			glBindVertexArray(0);
		}

	private:
		GLuint id;
};
