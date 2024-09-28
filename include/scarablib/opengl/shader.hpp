#pragma once

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include "scarablib/proper/vector/vec4.hpp"

// OpenGL shader struct
struct Shader {
		// Construct a shader using vertex and fragment shaders content
		Shader(const char* vertex_source, const char* fragment_source);
		~Shader();

		// Enable shader
		inline void use() const {
			glUseProgram(this->id);
		}

		// Unbind shader program
		inline void unbind() const {
			glUseProgram(0);
		}


		// UNIFORMS //

	// Make a uniform using a matrix4f value
		inline void set_matrix4f(const char* unif, const glm::mat4& mat) const {
			GLint loc = glGetUniformLocation(this->id, unif);
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
		}

		// Make a uniform using a vector4f value
		inline void set_vector4f(const char* unif, const vec4<float>& vec) const {
			GLint loc = glGetUniformLocation(this->id, unif);
			glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
		}

		// Make a uniform using an int value
		inline void set_int(const char* unif, const GLint val) const {
			GLint loc = glGetUniformLocation(this->id, unif);
			glUniform1i(loc, val);
		}
	private:
		GLuint id;
};
