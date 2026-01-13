#include "scarablib/opengl/uniformbuffer.hpp"
#include "scarablib/proper/error.hpp"
#include "scarablib/typedef.hpp"
#include <cstring>

UniformBuffer::UniformBuffer(const size_t size, const uint32 binding_point)
	: size(size), binding(binding_point) {

#if !defined(BUILD_OPGL30)
	glCreateBuffers(1, &this->id);
	// Initializes buffer object
	glNamedBufferStorage(this->id,
		size,    // Buffer size
		nullptr, // Initial data pointer
		GL_MAP_WRITE_BIT        // CPU Writes to buffer
		| GL_MAP_PERSISTENT_BIT // Remain valid until buffer is destroyed
		| GL_MAP_COHERENT_BIT   // CPU Writes immediately visible to GPU
	);
	// Map buffer parts
	this->mapped = glMapNamedBufferRange(this->id,
		0,    // Start Offset
		size, // Bytes to map
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
	);
#else
	glGenBuffers(1, &this->id);
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);

	glBufferStorage(GL_UNIFORM_BUFFER, size,
		nullptr,
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
	);

	this->mapped = glMapBufferRange(GL_UNIFORM_BUFFER,
		0, size,
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT
	);
#endif

	if(!mapped) {
		throw ScarabError("Failed to map Uniform Buffer");
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, this->binding, this->id);
}

UniformBuffer::~UniformBuffer() noexcept {
#if !defined(BUILD_OPGL30)
	glUnmapNamedBuffer(this->id);
#else
	glBindBuffer(GL_UNIFORM_BUFFER, this->id);
	glUnmapBuffer(GL_UNIFORM_BUFFER);
#endif
	glDeleteBuffers(1, &this->id);
}


void UniformBuffer::update(const void* data, const size_t size, const uint32 offset) const {
	if(offset + size > this->size) {
		throw ScarabError("Uniform Buffer overflow");
	}

	// - Write updated uniform data directly into persistently mapped Uniform Buffer memory
	//   (GL_MAP_WRITE_BIT)
	// - CPU-side write into GPU-visible uniform buffer memory (persistent + coherent mapping)
	//   (GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT)
	std::memcpy(static_cast<uint8*>(mapped) + offset, data, size);
}
