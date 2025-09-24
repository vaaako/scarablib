#include "scarablib/opengl/vertexarraydsa.hpp"
#include "scarablib/proper/error.hpp"

VertexArrayDSA::~VertexArrayDSA() noexcept {
	glDeleteVertexArrays(1, &this->vao_id);
}

void VertexArrayDSA::link_attrib(const uint32 vao_id, const uint32 index, const uint32 size,
		const GLenum type, const uint32 offset, const uint32 binding_index, const bool normalized) const {

	glEnableVertexArrayAttrib(vao_id, index);
	glVertexArrayAttribBinding(vao_id, index, binding_index);

	switch (type) {
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_INT:
		case GL_UNSIGNED_INT:
			glVertexArrayAttribIFormat(vao_id, index, size, type, offset);
			break;

		case GL_HALF_FLOAT:
		case GL_FLOAT:
		case GL_DOUBLE:
			glVertexArrayAttribFormat(vao_id, index, size, type,
									  normalized ? GL_TRUE : GL_FALSE, offset);
			break;

		default:
			throw ScarabError("[%s] Type (%i) enum not found", __func__, type);
			break;
	}
}
