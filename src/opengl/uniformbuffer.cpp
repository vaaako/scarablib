#include "scarablib/opengl/uniformbuffer.hpp"
#include "scarablib/proper/error.hpp"

UniformBuffer::UniformBuffer(const size_t size, const uint32 binding_point, bool dynamic) noexcept
	: size(size), binding(binding_point) {
#if !defined(BUILD_OPGL30)
	glCreateBuffers(1, &this->id);
	glNamedBufferData(this->id, size, nullptr, (dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
#else
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
	glBufferData(GL_UNIFORM_BUFFER, size, nullptr, (dynamic) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
#endif
	glBindBufferBase(GL_UNIFORM_BUFFER, this->binding, this->id);
}

UniformBuffer::~UniformBuffer() noexcept {
	glDeleteBuffers(1, &this->id);
}


void UniformBuffer::update(const void* data, const size_t size, const uint32 offset) const {
	if(offset + size > this->size) {
		throw ScarabError("UBO overflow");
	}

#if !defined(BUILD_OPGL30)
	glNamedBufferSubData(this->id, offset, size, data);
#else
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
	glBufferSubData(GL_UNIFORM_BUFFER, offset, size, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
#endif
}
