#pragma once

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>
#include "scarablib/opengl/shader.hpp"
#include "scarablib/typedef.hpp"
#include "scarablib/gfx/color.hpp"

// OpenGL shader object
class ShaderProgram {
	friend class ShaderManager;

	public:
		// Contruct shader giving a vector of all desired shaders to attach to the program
		ShaderProgram(const std::vector<std::shared_ptr<Shader>>& shaders);
		// Must be shared_ptr because this will own the weak_ptr of ShaderManager
		~ShaderProgram() noexcept;

		// Disable copy and moving
		ShaderProgram(const ShaderProgram&) noexcept = delete;
		ShaderProgram& operator=(const ShaderProgram&) noexcept = delete;
		ShaderProgram(ShaderProgram&&) noexcept = delete;
		ShaderProgram& operator=(ShaderProgram&&) noexcept = delete;

		// Returns the id of shader program
		inline uint32 get_programid() const noexcept {
			return this->programid;
		}

		// Returns the hash of shader program.
		// Will return 0 if this Shader Program was not created using the ShaderManager
		inline size_t get_hash() const noexcept {
			return this->hash;
		}

		// Enables the shader program
		inline void use() const noexcept {
			glUseProgram(this->programid);
		}

		// Disables the shader program
		inline void unbind() const noexcept {
			glUseProgram(0);
		}

		// Returns an attached shader by id.
		// Returns nullptr if not found
		std::shared_ptr<Shader> get_shader(const uint32 id);
		// Returns an attached shader by type.
		// Returns nullptr if not found
		std::shared_ptr<Shader> get_shader(const Shader::Type type);

		// Swaps a attached shader to another.
		// This method will look for this type of shader and swap it by the new id
		void swap_shader(const uint32 id, const Shader::Type type);
		

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
		size_t hash = 0; // Only ShaderManager changes this value
		// Must be shared_ptr because this will own the weak_ptr of ShaderManager
		std::vector<std::shared_ptr<Shader>> attached_shaders;
};
