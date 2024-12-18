#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/error.hpp"
#include <cstddef>

Shader::Shader(const char* vertex_source, const char* fragment_source) {
	// Compile vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if (vertex_shader == 0) {
		throw ScarabError("Failed to create vertex shader.");
	}
	glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
	glCompileShader(vertex_shader);

	// Check vertex shader error
	GLint success;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
		throw ScarabError("Vertex shader compilation failed: %s", info_log);
	}

	// Compile fragment shader
	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	if (fragment_shader == 0) {
		throw ScarabError("Failed to create fragment shader.");
	}
	glShaderSource(fragment_shader, 1, &fragment_source, nullptr);
	glCompileShader(fragment_shader);

	// Check fragment shader error
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
		throw ScarabError("Fragment shader compilation failed: %s", info_log);
	}

	// Create shader program
	this->id = glCreateProgram(); // Set program ID
	if (this->id == 0) {
		throw ScarabError("Failed to create shader program.");
	}

	glAttachShader(this->id, vertex_shader);
	glAttachShader(this->id, fragment_shader);
	glLinkProgram(this->id);

	// Check link error
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar info_log[512];
		glGetProgramInfoLog(this->id, 512, NULL, info_log);
		throw ScarabError("Error on linking shaders: %s", info_log);
	}

	// Clean up shaders
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}
Shader::~Shader() {
	glDeleteProgram(this->id);
}

