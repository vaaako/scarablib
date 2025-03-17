#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/proper/log.hpp"
#include "scarablib/utils/file.hpp"
#include <cstddef>

Shader::Shader(const char* vertex, const char* fragment, const bool is_file_path) {
	const char* vertex_source;
	const char* fragment_source;

	// Get content from file (if its a file path)
	if(is_file_path) {
		std::string exec_path = FileHelper::executable_path();
		LOG_DEBUG("Executable path: %s", exec_path.c_str());
		vertex_source   = FileHelper::read_file_char(exec_path + "/" + vertex);
		fragment_source = FileHelper::read_file_char(exec_path + "/" + fragment);

		if(vertex_source == nullptr) {
			throw ScarabError("Failed to read vertex shader files (%s)", vertex);
		}

		if(fragment_source == nullptr) {
			throw ScarabError("Failed to read fragment shader files (%s)", fragment);
		}

	} else {
		vertex_source = vertex;
		fragment_source = fragment;
	}

	// Compile vertex shader
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	if(vertex_shader == 0) {
		throw ScarabError("Failed to create vertex shader.");
	}
	glShaderSource(vertex_shader, 1, &vertex_source, nullptr);
	glCompileShader(vertex_shader);

	// Check vertex shader error
	GLint success;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);

		if(is_file_path) {
			throw ScarabError("Vertex shader (%s) compilation failed: %s", vertex, info_log);
		}
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
	if(!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);

		if(is_file_path) {
			throw ScarabError("Fragment shader (%s) compilation failed: %s", fragment, info_log);
		}
		throw ScarabError("Fragment shader compilation failed: %s", info_log);
	}

	// Create shader program
	this->id = glCreateProgram(); // Set program ID
	if(this->id == 0) {
		throw ScarabError("Failed to create shader program.");
	}

	glAttachShader(this->id, vertex_shader);
	glAttachShader(this->id, fragment_shader);
	glLinkProgram(this->id);

	// Check link error
	glGetProgramiv(this->id, GL_LINK_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetProgramInfoLog(this->id, 512, NULL, info_log);
		throw ScarabError("Error on linking shaders: %s", info_log);
	}

	// Clean up shaders
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	if(is_file_path) {
		delete[] vertex_source;
		delete[] fragment_source;
	}
}

Shader::~Shader() noexcept {
	glDeleteProgram(this->id);
}

