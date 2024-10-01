#include "scarablib/opengl/vbo.hpp"

VBO::VBO() {
	glGenBuffers(1, &this->id);
}

VBO::~VBO() {
	glDeleteBuffers(1, &this->id);
}

void VBO::alloc_data(const uint64 size, const void* data, const GLenum drawtype) {
	this->bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, drawtype);
	this->unbind();
}

void VBO::link_attrib(const uint32 index, const uint32 dimension, const int total_byte_size, const uint32 offset) {
	this->bind();
	glVertexAttribPointer(index, dimension, GL_FLOAT, GL_FALSE, total_byte_size, (void*)offset);
	glEnableVertexAttribArray(index);
	this->unbind();
}

