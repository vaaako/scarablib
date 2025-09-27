#pragma once

#include <glm/ext.hpp>
#include <glm/mat4x4.hpp>
#include <memory>
#include <vector>
#include "scarablib/typedef.hpp"
#include "scarablib/gfx/color.hpp"

// OpenGL shader object
class ShaderProgram {
	friend class ShaderManager;

	public:
		// Supported shader versions
		enum class Type {
			Vertex   = GL_VERTEX_SHADER,
			Fragment = GL_FRAGMENT_SHADER
		};

		// Struct used for Shader object
		// struct Shader {
		// 	uint32 id   = 0;
		// 	uint32 hash = 0;
		// 	ShaderProgram::Type type;
		// };

		// Shader information to pass to program.
		// Use `existing_id` if you want to use an existing ID
		struct ShaderInfo {
			// Shader's source
			const char* source = nullptr;
			// Type of the shader
			ShaderProgram::Type type;
			// Use an existing shader instead of a new source code
			std::shared_ptr<uint32> existing_id = nullptr;
		};

		// Contruct shader giving a vector of all desired shaders to use
		ShaderProgram(const std::vector<ShaderInfo>& infos);
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

		// Enable shader program
		inline void use() const noexcept {
			glUseProgram(this->programid);
		}

		// Disable shader program
		inline void unbind() const noexcept {
			glUseProgram(0);
		}

		inline std::vector<>


		// Utility function for users who might want to compile a single shader
		// without creating a full program.
		// It returns a `shared_ptr<uint32>` because this method is usually used inside `ShaderManager`.
		// This `shared_ptr` contains a custom destructor
		static std::shared_ptr<uint32> compile_shader(const char* source, const ShaderProgram::Type type);



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
		uint32 hash;
		std::vector<std::shared_ptr<uint32>> attached_shaders;
};
