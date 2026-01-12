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
	friend class ResourcesManager;

	public:
		// Contruct shader giving a vector of all desired shaders to attach to the program
		ShaderProgram(const std::vector<std::shared_ptr<Shader>>& shaders);
		// Must be shared_ptr because this will own the weak_ptr of ResourcesManager
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
		// Will return 0 if this Shader Program was not created using the ResourcesManager
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
		#if !defined(BUILD_OPGL30)
			glUniformMatrix4fv(glGetUniformLocation(this->programid, unif), static_cast<GLsizei>(index), GL_FALSE, glm::value_ptr(mat));
		#else
			const GLint loc = glGetUniformLocation(this->programid, unif);
			if (loc >= 0) {
				glProgramUniformMatrix4fv(this->programid, loc, static_cast<GLsizei>(index), GL_FALSE, glm::value_ptr(mat));
			}
		#endif
		}

		// Sends a vector of ints to the shader
		inline void set_ivec(const char* unif, const std::vector<int>& vec, const uint32 index = 1) const noexcept {
		#if !defined(BUILD_OPGL30)
			glUniform1iv(glGetUniformLocation(this->programid, unif), static_cast<GLsizei>(index), vec.data());
		#else
			const GLint loc = glGetUniformLocation(this->programid, unif);
			if (loc >= 0) {
				glProgramUniform1iv(this->programid, loc, static_cast<GLsizei>(index), vec.data());
			}
		#endif
		}

		// Sends color type as vec4f to the shader
		inline void set_color(const char* unif, const Color& color) const noexcept {
		#if !defined(BUILD_OPGL30)
			glUniform4f(glGetUniformLocation(this->programid, unif),
				color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, color.alpha / 255.0f);
		#else
			const GLint loc = glGetUniformLocation(this->programid, unif);
			if(loc >= 0) {
				glProgramUniform4f(this->programid, loc,
					color.red / 255.0f, color.green / 255.0f, color.blue / 255.0f, color.alpha / 255.0f);
			}
		#endif
		}
		// Division on CPU side is better
		// Otherwhise GPU would divide for each vertice

		// Sends a vec2f uniform to the shader
		inline void set_vector2f(const char* unif, const vec2<float>& vec) const noexcept {
		#if !defined(BUILD_OPGL30)
			glUniform2f(glGetUniformLocation(this->programid, unif), vec.x, vec.y);
		#else
			const GLint loc = glGetUniformLocation(this->programid, unif);
			if(loc >= 0) {
				glProgramUniform2f(this->programid, loc, vec.x, vec.y);
			}
		#endif
		}

		// Sends a vec3f uniform to the shader
		inline void set_vector3f(const char* unif, const vec3<float>& vec) const noexcept {
		#if !defined(BUILD_OPGL30)
			glUniform3f(glGetUniformLocation(this->programid, unif), vec.x, vec.y, vec.z);
		#else
			const GLint loc = glGetUniformLocation(this->programid, unif);
			if(loc >= 0) {
				glProgramUniform3f(this->programid, loc, vec.x, vec.y, vec.z);
			}
		#endif
		}

		// Sends a vec4f uniform to the shader
		inline void set_vector4f(const char* unif, const vec4<float>& vec) const noexcept {
		#if !defined(BUILD_OPGL30)
			glUniform4f(glGetUniformLocation(this->programid, unif), vec.x, vec.y, vec.z, vec.w);
		#else
			const GLint loc = glGetUniformLocation(this->programid, unif);
			if(loc >= 0) {
				glProgramUniform4f(this->programid, loc, vec.x, vec.y, vec.z, vec.w);
			}
		#endif
		}

		// Sends an int uniform to the shader
		inline void set_int(const char* unif, const int val) const noexcept {
		#if !defined(BUILD_OPGL30)
			glUniform1i(glGetUniformLocation(this->programid, unif), val);
		#else
			const GLint loc = glGetUniformLocation(this->programid, unif);
			if(loc >= 0) {
				glProgramUniform1i(this->programid, loc, val);
			}
		#endif
		}

		// Sends a float uniform to the shader
		inline void set_float(const char* unif, const float val) const noexcept {
		#if !defined(BUILD_OPGL30)
			glUniform1f(glGetUniformLocation(this->programid, unif), val);
		#else
			const GLint loc = glGetUniformLocation(this->programid, unif);
			if(loc >= 0) {
				glProgramUniform1f(this->programid, loc, val);
			}
		#endif
		}

	private:
		std::vector<std::shared_ptr<Shader>> attached_shaders;
		// I need to store as a Shader struct and not only the IDs so the weak_ptr doesnt get expired
		// I also need as struct because ResourcesManager uses this when making a custom shader
		
		size_t hash = 0; // Only ResourcesManager changes this value
		GLuint programid;
};
