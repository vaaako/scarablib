#include "scarablib/opengl/shader_program.hpp"
#include "scarablib/proper/error.hpp"
#include <algorithm>
#include <cstddef>

ShaderProgram::ShaderProgram(const std::vector<std::shared_ptr<Shader>>& shaders) {
	if(shaders.empty()) {
		throw ScarabError("No shaders provided to create a program");
	}

	for(const auto& shader : shaders) {
		if(shader == nullptr || shader->id == 0) {
			throw ScarabError("\"shader\" object is nullptr or contains an invalid id");
		}
	}

	uint32 programid = glCreateProgram();
	if(programid == 0) {
		throw ScarabError("Failed to create shader program");
	}

	for(const auto& shader : shaders) {
		glAttachShader(programid, shader->id);
	}
	glLinkProgram(programid);

	// Detach shaders (not needed anymore)
	for(const auto& shader : shaders) {
		glDetachShader(programid, shader->id);
		// glDeleteShader(shaderid);
	}

	// Check for linking error
	GLint success;
	glGetProgramiv(programid, GL_LINK_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetProgramInfoLog(programid, 512, NULL, info_log);
		glDeleteShader(programid); // Clean up the failed program
		throw ScarabError("Error Linking shaders: \n%s", info_log);
	}

	this->programid = programid;
	this->attached_shaders = shaders; // transfer ownership of weak_ptr of "shaders"
}

ShaderProgram::~ShaderProgram() noexcept {
	glDeleteProgram(this->programid);
	// Has no effect since its all shared_ptr, but i like to have it here
	this->attached_shaders.clear();
}

std::shared_ptr<Shader> ShaderProgram::get_shader(const uint32 id) {
	// Find and remove existing shader
	auto it = std::find_if(this->attached_shaders.begin(), this->attached_shaders.end(), [&](const std::shared_ptr<Shader>& s){
		return s->id == id;
	});
	return (it != attached_shaders.end()) ? *it : nullptr;
}

std::shared_ptr<Shader> ShaderProgram::get_shader(const Shader::Type type) {
	// Find and remove existing shader
	auto it = std::find_if(this->attached_shaders.begin(), this->attached_shaders.end(), [&](const std::shared_ptr<Shader>& s){
		return s->type == type;
	});
	return (it != attached_shaders.end()) ? *it : nullptr;
}

void ShaderProgram::swap_shader(const uint32 id, const Shader::Type type) {
	// Remove existing
	if(std::shared_ptr<Shader> existing = this->get_shader(type)) {
		this->attached_shaders.erase(
			std::remove(this->attached_shaders.begin(), this->attached_shaders.end(), existing), this->attached_shaders.end()
		);
	}

	// Link shader
	glAttachShader(programid, id);
	glLinkProgram(programid);
	glDetachShader(programid, id);

	// Check for linking error
	GLint success;
	glGetProgramiv(programid, GL_LINK_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetProgramInfoLog(programid, 512, NULL, info_log);
		glDeleteShader(programid); // Clean up the failed program
		throw ScarabError("Error Linking shaders: \n%s", info_log);
	}
}
