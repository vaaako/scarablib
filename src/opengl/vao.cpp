#include "scarablib/opengl/vao.hpp"

VAO::VAO() {
	glGenVertexArrays(1, &this->id);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &this->id);
}
