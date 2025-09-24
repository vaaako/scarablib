#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"

VertexArray::~VertexArray() noexcept {
	glDeleteVertexArrays(1, &this->vao_id);
	glDeleteBuffers(1, &this->vbo_id);
	if(this->ebo_id != 0) {
		glDeleteBuffers(1, &this->ebo_id);
	}
}

void VertexArray::alloc_data(const uint64 size, const void* data, const GLenum usage) const noexcept {
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizei>(size), data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::alloc_data(const std::vector<float>& data, const GLenum usage) const noexcept {
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), data.data(), usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexArray::link_attrib(const uint32 index, const uint32 size, const GLenum type,
				const uint32 total_byte_size, const uint32 offset, const bool normalized) const {

	glBindVertexArray(this->vao_id);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);

	glEnableVertexAttribArray(index);
	switch (type) {
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_INT:
		case GL_UNSIGNED_INT:
			glVertexAttribIPointer(index, static_cast<GLint>(size), type,
					static_cast<GLsizei>(total_byte_size), reinterpret_cast<void*>(offset));
			break;

		case GL_HALF_FLOAT:
		case GL_FLOAT:
		case GL_DOUBLE:
			glVertexAttribPointer(index, static_cast<GLint>(size), type, normalized ? GL_TRUE : GL_FALSE,
					static_cast<GLsizei>(total_byte_size), reinterpret_cast<void*>(offset));
			break;

		default:
			throw ScarabError("[%s] Type (%i) enum not found", __func__, type);
			break;
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}




/*
BIND
ALLOC DATA <- (first being used)
LINK ATTR  <- (all that needs)
UNBIND
*/

