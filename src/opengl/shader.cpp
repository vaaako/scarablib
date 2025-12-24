#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/utils/hash.hpp"

Shader::Shader(const char* source, const Shader::Type type)
	: hash(ScarabHash::hash_make(std::string(source))) {

	// Compile vertex shader
	this->id = glCreateShader((int)type);
	if(this->id == 0) {
		throw ScarabError("Failed to compile shader.");
	}
	glShaderSource(this->id, 1, &source, nullptr);
	glCompileShader(this->id);

	// Check for compilation error
	GLint success;
	glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
	if(!success) {
		GLchar info_log[512];
		glGetShaderInfoLog(this->id, 512, NULL, info_log);
		glDeleteShader(this->id); // Clean up the failed shader
		throw ScarabError("Error Compiling  %s: \n%s",
			((int)type == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT",
			info_log
		);
	}
}

Shader::~Shader() noexcept {
	glDeleteShader(this->id);
}
