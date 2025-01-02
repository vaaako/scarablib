#pragma once

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"

// OpenGL shader struct
struct Shader {
		// Construct a shader using vertex and fragment shaders content
		Shader(const char* vertex_source, const char* fragment_source);
		~Shader();

		// Disable copy and moving
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		Shader(Shader&&) = delete;
		Shader& operator=(Shader&&) = delete;

		// Get shader's ID
		inline GLuint get_id() const {
			return this->id;
		}

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
		inline void set_matrix4f(const char* unif, const glm::mat4& mat, const uint32 index = 1) const {
			glUniformMatrix4fv(glGetUniformLocation(this->id, unif), static_cast<GLsizei>(index), GL_FALSE, glm::value_ptr(mat));
		}

		// Set value of a ivec uniform
		inline void set_ivec(const char* unif, const std::vector<int>& vec, const uint32 index = 1) const {
			glUniform1iv(glGetUniformLocation(this->id, unif), static_cast<GLsizei>(index), &vec[0]);
		}

		// Set value of a vec2f uniform
		inline void set_vector4f(const char* unif, const vec4<float>& vec) const {
			glUniform4f(glGetUniformLocation(this->id, unif), vec.x, vec.y, vec.z, vec.w);
		}

		inline void set_color(const char* unif, const Color& color) const {
			glUniform4f(glGetUniformLocation(this->id, unif), color.red, color.green, color.blue, color.alpha);
		}

		// Set value of a vec4f uniform
		inline void set_vector2f(const char* unif, const vec2<float>& vec) const {
			glUniform2f(glGetUniformLocation(this->id, unif), vec.x, vec.y);
		}

		// Set value of a int uniform
		inline void set_int(const char* unif, const GLint val) const {
			glUniform1i(glGetUniformLocation(this->id, unif), val);
		}

		// Set value of a float uniform
		inline void set_float(const char* unif, const GLfloat val) const {
			glUniform1f(glGetUniformLocation(this->id, unif), val);
		}
	private:
		GLuint id;
};
