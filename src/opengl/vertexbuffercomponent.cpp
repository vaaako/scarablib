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
	this->attributes.push_back(VertexAttribute {
		// .location        = static_cast<uint32>(this->attributes.size()),
		.component_count = count,
		.type            = type,
		.normalized      = normalized,
		.offset          = this->stride
	});
	this->stride += count * sizeof(float);
}
