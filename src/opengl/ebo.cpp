#include "scarablib/opengl/ebo.hpp"

EBO::EBO(const uint32 id) noexcept : id(id) {}

EBO::~EBO() noexcept {
	glDeleteBuffers(1, &this->id);
}

