#pragma once

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
#include "scarablib/typedef.hpp"
#include "scarablib/gfx/color.hpp"

// OpenGL shader object
class ShaderProgram {
	public:
		// Construct a shader using vertex and fragment shaders source code
		ShaderProgram(const char* vertex_source, const char* fragment_source);
		// This constructor is used when making a program out of an existing shader
		ShaderProgram(const bool isvertex, const uint32 shaderid, const char* source);
		~ShaderProgram() noexcept;

		// Disable copy and moving
		ShaderProgram(const ShaderProgram&) noexcept = delete;
		ShaderProgram& operator=(const ShaderProgram&) noexcept = delete;
		ShaderProgram(ShaderProgram&&) noexcept = delete;
		ShaderProgram& operator=(ShaderProgram&&) noexcept = delete;

		// Returns id of shader program
		inline uint32 get_programid() const noexcept {
			return this->programid;
		}

		// Returns id of fragment shader
		inline uint32 get_vertexid() const noexcept {
			return this->vertexid;
		}

		// // Returns id of vertex shader
		// inline uint32 get_fragmentid() const noexcept {
		// 	return this->fragmentid;
		// }

		// Enable shader program
		inline void use() const noexcept {
			glUseProgram(this->programid);
		}

		// Disable shader program
		inline void unbind() const noexcept {
			glUseProgram(0);
		}


		// Compiles a shader and returns its ID to use it when creating the shader.
		// If an ID for this shader already exists, it will be replaced
		void compile_shader(const char* source, const bool isvertex = true);

		// Uses vertex and fragment shader IDs (created with ShaderProgram::compile_shader) to make Shader Program.
		void link_program();


		// -- UNIFORMS

		// Sends a mat4f to the shader
		inline void set_matrix4f(const char* unif, const glm::mat4& mat, const uint32 index = 1) const noexcept {
			glUniformMatrix4fv(glGetUniformLocation(this->programid, unif), static_cast<GLsizei>(index), GL_FALSE, glm::value_ptr(mat));
		}

		// Sends a vector of ints to the shader
		inline void set_ivec(const char* unif, const std::vector<int>& vec, const uint32 index = 1) const noexcept {
			glUniform1iv(glGetUniformLocation(this->programid, unif), static_cast<GLsizei>(index), &vec[0]);
		}

		// Sends color type as vec4f to the shader
		inline void set_color(const char* unif, const Color& color) const noexcept {
			glUniform4f(glGetUniformLocation(this->programid, unif), color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, color.alpha / 255.0f);
		}
		// Division on CPU side is better
		// Otherwhise GPU would divide for each vertice

		// Sends a vec2f uniform to the shader
		inline void set_vector2f(const char* unif, const vec2<float>& vec) const noexcept {
			glUniform2f(glGetUniformLocation(this->programid, unif), vec.x, vec.y);
		}

		// Sends a vec3f uniform to the shader
		inline void set_vector3f(const char* unif, const vec3<float>& vec) const noexcept {
			glUniform3f(glGetUniformLocation(this->programid, unif), vec.x, vec.y, vec.z);
		}

		// Sends a vec4f uniform to the shader
		inline void set_vector4f(const char* unif, const vec4<float>& vec) const noexcept {
			glUniform4f(glGetUniformLocation(this->programid, unif), vec.x, vec.y, vec.z, vec.w);
		}

		// Sends a int uniform to the shader
		inline void set_int(const char* unif, const int val) const noexcept {
			glUniform1i(glGetUniformLocation(this->programid, unif), val);
		}

		// Sets value of a float value uniform and sends it to the shader
		inline void set_float(const char* unif, const float val) const noexcept {
			glUniform1f(glGetUniformLocation(this->programid, unif), val);
		}
	private:
		uint32 programid;
		uint32 vertexid   = 0;
		uint32 fragmentid = 0;
};
