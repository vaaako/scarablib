#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/typedef.hpp"

VertexArray::VertexArray(const void* data, const size_t capacity, const size_t vertex_size, const bool dynamic) noexcept
	: vsize(vertex_size), length(0) {

#if !defined(BUILD_OPGL30)
	glCreateVertexArrays(1, &this->vao_id);
	glCreateBuffers(1, &this->vbo_id);
#else
	glGenVertexArrays(1, &this->vao_id);
	glGenBuffers(1, &this->vbo_id);
#endif
	this->alloc_data(data, capacity, dynamic);
}

VertexArray::~VertexArray() noexcept {
	if(this->ebo_id != 0) {
		glDeleteBuffers(1, &this->ebo_id);
	}
	glDeleteBuffers(1, &this->vbo_id);
	glDeleteVertexArrays(1, &this->vao_id);
}

void VertexArray::alloc_data(const void* data, const size_t capacity, const bool dynamic) const noexcept {
#if !defined(BUILD_OPGL30)
	glNamedBufferData(
		this->vbo_id,
		this->vsize * capacity,
		data,
		(dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
	);
#else
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	glBufferData(GL_ARRAY_BUFFER, this->vsize * capacity, data, (dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

void VertexArray::update_data(const void* data, size_t size) noexcept {
#if !defined(BUILD_OPGL30)
	glNamedBufferSubData(
		this->vbo_id,
		0,
		static_cast<GLsizeiptr>(size),
		data
	);
#else
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		0,
		static_cast<GLsizeiptr>(size),
		data
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

/*
BIND
ALLOC DATA <- (first being used)
LINK ATTR  <- (all that needs)
UNBIND
*/

