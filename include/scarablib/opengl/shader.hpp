#pragma once

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include "scarablib/typedef.hpp"
#include "scarablib/gfx/color.hpp"

// OpenGL shader object
struct Shader {
		// Construct a shader using vertex and fragment shaders code
		Shader(const char* vertex_source, const char* fragment_source);
		~Shader() noexcept;

		// Disable copy and moving
		Shader(const Shader&) noexcept = delete;
		Shader& operator=(const Shader&) noexcept = delete;
		Shader(Shader&&) noexcept = delete;
		Shader& operator=(Shader&&) noexcept = delete;

		// Returns shader ID
		inline GLuint get_id() const noexcept {
			return this->id;
		}

		// Enable shader program
		inline void use() const noexcept {
			glUseProgram(this->id);
		}

		// Disable shader program
		inline void unbind() const noexcept {
			glUseProgram(0);
		}


		// UNIFORMS //

		// Sends a mat4f oto the shader
		inline void set_matrix4f(const char* unif, const glm::mat4& mat, const uint32 index = 1) const noexcept {
			glUniformMatrix4fv(glGetUniformLocation(this->id, unif), static_cast<GLsizei>(index), GL_FALSE, glm::value_ptr(mat));
		}

		// Sends a vector of ints to the shader
		inline void set_ivec(const char* unif, const std::vector<int>& vec, const uint32 index = 1) const noexcept {
			glUniform1iv(glGetUniformLocation(this->id, unif), static_cast<GLsizei>(index), &vec[0]);
		}

		// Sends color type as vec4f to the shader
		inline void set_color(const char* unif, const Color& color) const noexcept {
			glUniform4f(glGetUniformLocation(this->id, unif), color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, color.alpha / 255.0f);
		}
		// Divide by 255 in CPU is better
		// If divide here it would be a division per vertex

		// Sends a vec2f uniform to the shader
		inline void set_vector2f(const char* unif, const vec2<float>& vec) const noexcept {
			glUniform2f(glGetUniformLocation(this->id, unif), vec.x, vec.y);
		}

		// Sends a vec3f uniform to the shader
		inline void set_vector3f(const char* unif, const vec3<float>& vec) const noexcept {
			glUniform3f(glGetUniformLocation(this->id, unif), vec.x, vec.y, vec.z);
		}

		// Sends a vec4f uniform to the shader
		inline void set_vector4f(const char* unif, const vec4<float>& vec) const noexcept {
			glUniform4f(glGetUniformLocation(this->id, unif), vec.x, vec.y, vec.z, vec.w);
		}

		// Sends a int uniform to the shader
		inline void set_int(const char* unif, const int val) const noexcept {
			glUniform1i(glGetUniformLocation(this->id, unif), val);
		}

		// Sets value of a float value uniform and sends it to the shader
		inline void set_float(const char* unif, const float val) const noexcept {
			glUniform1f(glGetUniformLocation(this->id, unif), val);
		}
	private:
		GLuint id;
};
