#pragma once

#include <GL/glew.h>
#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include "scarablib/typedef.hpp"
#include "scarablib/types/color.hpp"

// OpenGL shader struct
struct Shader {
		// Construct a shader using vertex and fragment shaders content or file path.
		// is_file_path is true if vertex and fragment are file paths
		Shader(const char* vertex, const char* fragment, const bool is_file_path = false);
		~Shader() noexcept;

		// Disable copy and moving
		Shader(const Shader&) noexcept = delete;
		Shader& operator=(const Shader&) noexcept = delete;
		Shader(Shader&&) noexcept = delete;
		Shader& operator=(Shader&&) noexcept = delete;

		// Get shader's ID
		inline GLuint get_id() const noexcept {
			return this->id;
		}

		// Enable shader
		inline void use() const noexcept {
			glUseProgram(this->id);
		}

		// Unbind shader program
		inline void unbind() const noexcept {
			glUseProgram(0);
		}


		// UNIFORMS //

		// Sets value of a mat4f uniform and sends it to the shader
		inline void set_matrix4f(const char* unif, const glm::mat4& mat, const uint32 index = 1) const noexcept {
			glUniformMatrix4fv(glGetUniformLocation(this->id, unif), static_cast<GLsizei>(index), GL_FALSE, glm::value_ptr(mat));
		}

		// Sets value of a ivec uniform and sends it to the shader
		inline void set_ivec(const char* unif, const std::vector<int>& vec, const uint32 index = 1) const noexcept {
			glUniform1iv(glGetUniformLocation(this->id, unif), static_cast<GLsizei>(index), &vec[0]);
		}

		// Converts the color struct to a vec4f and sends it to the shader
		inline void set_color(const char* unif, const Color& color) const noexcept {
			glUniform4f(glGetUniformLocation(this->id, unif), color.red, color.green, color.blue, color.alpha);
		}

		// Sets value of a vec2f uniform and sends it to the shader
		inline void set_vector2f(const char* unif, const vec2<float>& vec) const noexcept {
			glUniform2f(glGetUniformLocation(this->id, unif), vec.x, vec.y);
		}

		// Sets value of a vec3f uniform and sends it to the shader
		inline void set_vector3f(const char* unif, const vec3<float>& vec) const noexcept {
			glUniform3f(glGetUniformLocation(this->id, unif), vec.x, vec.y, vec.z);
		}

		// Sets value of a vec4f uniform and sends it to the shader
		inline void set_vector4f(const char* unif, const vec4<float>& vec) const noexcept {
			glUniform4f(glGetUniformLocation(this->id, unif), vec.x, vec.y, vec.z, vec.w);
		}

		// Sets value of a int value uniform and sends it to the shader
		inline void set_int(const char* unif, const GLint val) const noexcept {
			glUniform1i(glGetUniformLocation(this->id, unif), val);
		}

		// Sets value of a float value uniform and sends it to the shader
		inline void set_float(const char* unif, const GLfloat val) const noexcept {
			glUniform1f(glGetUniformLocation(this->id, unif), val);
		}
	private:
		GLuint id;
};
