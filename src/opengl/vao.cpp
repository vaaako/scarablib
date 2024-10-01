#include "scarablib/opengl/vao.hpp"
#include "scarablib/proper/log.hpp"

VAO::VAO() {
	LOG_INFO_FN("VAO constructor");
	glGenVertexArrays(1, &this->id);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &this->id);
}
