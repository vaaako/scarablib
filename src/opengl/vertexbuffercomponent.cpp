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
