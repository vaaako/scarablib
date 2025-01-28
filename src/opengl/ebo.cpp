#include "scarablib/opengl/ebo.hpp"


EBO::EBO(const std::vector<uint32>& indices) noexcept {
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint32)), indices.data(), GL_STATIC_DRAW);
}

EBO::EBO(const std::vector<uint16>& indices) noexcept {
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizei>(indices.size() * sizeof(uint16)), indices.data(), GL_STATIC_DRAW);
}

EBO::~EBO() noexcept {
	glDeleteBuffers(1, &this->id);
}

