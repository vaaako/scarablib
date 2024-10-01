#pragma once

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include "scarablib/proper/vector/vec2.hpp"
#include "scarablib/proper/vector/vec4.hpp"

// OpenGL shader struct
struct Shader {
		// Construct a shader using vertex and fragment shaders content
		Shader(const char* vertex_source, const char* fragment_source);
		~Shader();

		// Disable copy and moving
		Shader(const Shader&) = delete;
		Shader(const Shader&&) = delete;

		// Enable shader
		inline void use() const {
			glUseProgram(this->id);
		}

		// Unbind shader program
		inline void unbind() const {
			glUseProgram(0);
		}


		// UNIFORMS //

		// Set value of a matrix4f uniform
		inline void set_matrix4f(const char* unif, const glm::mat4& mat) const {
			GLint loc = glGetUniformLocation(this->id, unif);
			glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
		}

		// Set value of a vec2f uniform
		inline void set_vector4f(const char* unif, const vec4<float>& vec) const {
			GLint loc = glGetUniformLocation(this->id, unif);
			glUniform4f(loc, vec.x, vec.y, vec.z, vec.w);
		}

		// Set value of a vec4f uniform
		inline void set_vector2f(const char* unif, const vec2<float>& vec) const {
			GLint loc = glGetUniformLocation(this->id, unif);
			glUniform2f(loc, vec.x, vec.y);
		}

		// Set value of a int uniform
		inline void set_int(const char* unif, const GLint val) const {
			GLint loc = glGetUniformLocation(this->id, unif);
			glUniform1i(loc, val);
		}

		// Set value of a float uniform
		inline void set_float(const char* unif, const GLfloat val) const {
			GLint loc = glGetUniformLocation(this->id, unif);
			glUniform1f(loc, val);
		}
	private:
		GLuint id;
};
