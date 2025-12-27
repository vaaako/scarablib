#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/typedef.hpp"

VertexArray::VertexArray(const size_t capacity, const size_t vertex_size, const bool dynamic_vertex) noexcept
	: length(0), vsize(vertex_size) {

#if !defined(BUILD_OPGL30)
	glCreateVertexArrays(1, &this->vao_id);
	glCreateBuffers(1, &this->vbo_id);

	glNamedBufferData(
		this->vbo_id,
		static_cast<GLsizeiptr>(capacity * vertex_size),
		nullptr,
		(dynamic_vertex) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
	);
#else
	glGenVertexArrays(1, &this->vao_id);
	glGenBuffers(1, &this->vbo_id);

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizei>(capacity),
		data.data(),
		(dynamic_vertex) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW
	);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
#endif
}

VertexArray::~VertexArray() noexcept {
	if(this->ebo_id != 0) {
		glDeleteBuffers(1, &this->ebo_id);
	}
	glDeleteBuffers(1, &this->vbo_id);
	glDeleteVertexArrays(1, &this->vao_id);
}


void VertexArray::update_vertices(const void* data, size_t size) noexcept {
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

