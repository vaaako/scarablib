#include "scarablib/opengl/shader_program.hpp"
#include "scarablib/proper/error.hpp"
#include <cstddef>

ShaderProgram::ShaderProgram(const std::vector<ShaderInfo>& infos) {
	if(infos.empty()) {
		throw ScarabError("No shader info provided to create a program");
	}

	for(const auto& info : infos) {
		if(info.existing_id != nullptr) {
			this->attached_shaders.push_back(info.existing_id);
		} else if(info.source != nullptr) {
			this->attached_shaders.push_back(this->compile_shader(info.source, info.type));
		} else {
			throw ScarabError("ShaderInfo must have either source or existing_id");
		}
	}

	uint32 programid = glCreateProgram();
	if(programid == 0) {
		throw ScarabError("Failed to create shader program");
	}

	for(const auto& shader_id : this->attached_shaders) {
		glAttachShader(programid, *shader_id);
	}
	glLinkProgram(programid);

	for(const auto& shader_id : this->attached_shaders) {
		glDetachShader(programid, *shader_id);
		// glDeleteShader(shaderid);
	}

	// Check for compilation error
	GLint success;
	glGetProgramiv(programid, GL_LINK_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetProgramInfoLog(programid, 512, NULL, info_log);
		glDeleteShader(programid); // Clean up the failed program
		throw ScarabError("Error Linking shaders: \n%s", info_log);
	}

	this->programid = programid;
}

ShaderProgram::~ShaderProgram() noexcept {
	glDeleteProgram(this->programid);
	// Has no effect since its all shared_ptr, but i like to have it here
	this->attached_shaders.clear();
}

std::shared_ptr<uint32> ShaderProgram::compile_shader(const char* source, const ShaderProgram::Type type) {
	// Compile vertex shader
	GLuint shaderid = glCreateShader((int)type);
	if(shaderid == 0) {
		throw ScarabError("Failed to compile shader.");
	}
	glShaderSource(shaderid, 1, &source, nullptr);
	glCompileShader(shaderid);

	// Check for compilation error
	GLint success;
	glGetShaderiv(shaderid, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(shaderid, 512, NULL, info_log);
		glDeleteShader(shaderid); // Clean up the failed shader
		throw ScarabError("Error Compiling  %s: \n%s",
			((int)type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT",
			info_log
		);
	}

	return std::shared_ptr<uint32>(new uint32(shaderid), [](uint32* id_to_delete) {
		if(id_to_delete != nullptr) {
			glDeleteShader(*id_to_delete);
			delete id_to_delete;
		}
	});
}
