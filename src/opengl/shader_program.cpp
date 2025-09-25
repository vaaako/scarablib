#include "scarablib/opengl/shader_program.hpp"
#include "scarablib/proper/error.hpp"
#include <cstddef>

ShaderProgram::ShaderProgram(const char* vertex_source, const char* fragment_source) {
	if(vertex_source == nullptr || fragment_source == nullptr) {
		throw ScarabError("Vertex or Fragment source is nullptr");
	}

	this->compile_shader(vertex_source, true);
	this->compile_shader(fragment_source, false);
	this->link_program();
}

ShaderProgram::ShaderProgram(const bool isvertex, const uint32 shaderid, const char* source) {
	if(source == nullptr) {
		throw ScarabError("Shader source is nullptr");
	}

	if(isvertex) {
		this->fragmentid = shaderid;
	} else {
		this->vertexid   = shaderid;
	}

	this->compile_shader(source, isvertex);
	this->link_program();
}

ShaderProgram::~ShaderProgram() noexcept {
	// Delete vertex
	glDetachShader(this->programid, this->vertexid);
	glDeleteShader(this->vertexid);
	// Delete fragment
	glDetachShader(this->programid, this->fragmentid);
	glDeleteShader(this->fragmentid);

	// Clean up shader program
	glDeleteProgram(this->programid);

	// TODO: Remove from shader manager
}

void ShaderProgram::compile_shader(const char* source, const bool isvertex) {
	// Compile vertex shader
	GLuint shader = glCreateShader(isvertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	if(shader == 0) {
		throw ScarabError("Failed to compile shader.");
	}

	glShaderSource(shader, 1, &source, nullptr);
	glCompileShader(shader);

	// Check shader error
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		glDeleteShader(shader); // Clean up the failed shader
		throw ScarabError("Shader compilation failed: \n%s", info_log);
	}
	
	if(isvertex) {
		if(this->vertexid != 0) {
			glDetachShader(this->programid, this->vertexid);
			glDeleteShader(this->vertexid);
		}
		this->vertexid = shader;
	} else {
		if(this->fragmentid != 0) {
			glDetachShader(this->programid, this->fragmentid);
			glDeleteShader(this->fragmentid);
		}
		this->fragmentid = shader;
	}
}

void ShaderProgram::link_program() {
	if(this->programid != 0) {
		glDeleteProgram(this->programid);
	}
	
	// Create shader program
	this->programid = glCreateProgram();
	if(this->programid == 0) {
		throw ScarabError("Failed to create shader program.");
	}

	// Link program to shaders
	glAttachShader(this->programid, this->vertexid);
	glAttachShader(this->programid, this->fragmentid);
	glLinkProgram(this->programid);

	// Check link error
	GLint success;
	glGetProgramiv(this->programid, GL_LINK_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetProgramInfoLog(this->programid, 512, NULL, info_log);
		// Clean up before throwing
		glDeleteProgram(this->programid);
		glDeleteShader(this->vertexid);
		glDeleteShader(this->fragmentid);
		throw ScarabError("Error on linking shaders: %s", info_log);
	}

	// Detach and delete shaders as they are now linked into the program
	// and no longer needed.
	// glDetachShader(this->programid, this->vertexid);
	// glDetachShader(this->programid, this->fragmentid);
	// glDeleteShader(this->vertexid);
	// glDeleteShader(this->fragmentid);
	// // Set IDs to 0 as they are no longer valid
	// this->vertexid = 0;
	// this->fragmentid = 0;
}
