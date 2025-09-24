#include "scarablib/opengl/vertexbuffercomponent.hpp"

VertexBufferComponent::~VertexBufferComponent() noexcept {
	if(this->hash != 0) {
		VAOManager::get_instance().release_vao(this->hash);
	}
}

// void VertexBufferComponent::add_attribute(const uint32 count, const GLenum type, const bool normalized) {
// 	this->attributes.push_back(VertexAttribute {
// 		// .location        = static_cast<uint32>(this->attributes.size()),
// 		.component_count = count,
// 		.type            = type,
// 		.normalized      = normalized,
// 		.offset          = this->stride
// 	});
// 	this->stride += count * sizeof(float);
// }
