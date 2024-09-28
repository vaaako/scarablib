#include "scarablib/opengl/ebo.hpp"

EBO::~EBO() {
	glDeleteBuffers(1, &this->id);
}

