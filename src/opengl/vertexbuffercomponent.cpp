#include "scarablib/opengl/vertexbuffercomponent.hpp"

VertexBufferComponent::~VertexBufferComponent() noexcept {
	if(this->vao != nullptr) {
		VAOManager::get_instance().release_vao(this->hash);
		delete this->vao;
	}

	if(this->vbo != nullptr) {
		delete this->vbo;
	}

	if(this->ebo != nullptr) {
		delete this->ebo;
	}
}

void VertexBufferComponent::add_attribute(const uint32 count, const GLenum type, const bool normalized) {
	// Plus 2 because position and texuv are always at location 0 and 1
	const uint32 location = 2 + this->attributes.size();
	this->attributes.push_back({ location, count, type, normalized, this->stride });
	this->stride += count * sizeof(float);
}
