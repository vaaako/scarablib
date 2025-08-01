#include "scarablib/opengl/vbo.hpp"

VBO::VBO(const uint32 id) noexcept {
	if(id != 0) {
		this->id = id;
		return;
	}
	glGenBuffers(1, &this->id);
}

VBO::~VBO() noexcept {
	glDeleteBuffers(1, &this->id);
}

void VBO::alloc_data(const uint64 size, const void* data, const GLenum usage) const noexcept {
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(size), data, usage);
}

void VBO::alloc_data(const std::vector<float>& data, const GLenum usage) const noexcept {
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), usage);
}

void VBO::link_attrib(const uint32 index, const uint32 count, const uint32 total_byte_size,
		const uint32 offset, const GLenum type, const bool normalized) const noexcept {
	glVertexAttribPointer(index, static_cast<GLint>(count), type, normalized,
			static_cast<GLsizei>(total_byte_size), reinterpret_cast<void*>(offset));
	glEnableVertexAttribArray(index);
}

/*

BIND
ALLOC DATA <- (first being used)
LINK ATTR  <- (all that needs)
UNBIND

*/

