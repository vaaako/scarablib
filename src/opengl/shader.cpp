#include "scarablib/opengl/shader.hpp"
#include "scarablib/proper/error.hpp"

Shader::Shader(const char* source, const Shader::Type type) {
	if(source == nullptr) {
		throw ScarabError("Shader source is empty");
	}

	// Compile vertex shader
	if((this->id = glCreateShader((int)type)) == 0) {
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
			(type == Shader::Type::Vertex) ? "VERTEX" : "FRAGMENT",
			info_log
		);
	}
}

Shader::~Shader() noexcept {
	glDeleteShader(this->id);
}
