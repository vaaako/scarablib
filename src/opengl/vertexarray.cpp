#include "scarablib/opengl/vertexarray.hpp"
#include "scarablib/typedef.hpp"

VertexArray::~VertexArray() noexcept {
	if(this->ebo_id != 0) {
		glDeleteBuffers(1, &this->ebo_id);
	}
	glDeleteBuffers(1, &this->vbo_id);
	glDeleteVertexArrays(1, &this->vao_id);
}

/*
BIND
ALLOC DATA <- (first being used)
LINK ATTR  <- (all that needs)
UNBIND
*/

