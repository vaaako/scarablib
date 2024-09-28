#include "scarablib/opengl/vbo.hpp"

VBO::VBO() {
	glGenBuffers(1, &this->id);
}

VBO::~VBO() {
	glDeleteBuffers(1, &this->id);
}

void VBO::alloc_data(const uint64 size, const void* data) {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	this->unbind();
}

void VBO::link_attrib(const uint32 index, const uint32 dimension, const int total_byte_size, const uint32 offset) {
	this->bind();
	glVertexAttribPointer(index, dimension, GL_FLOAT, GL_FALSE, total_byte_size, (void*)(offset * sizeof(float)));
	glEnableVertexAttribArray(index);
	this->unbind();
}

