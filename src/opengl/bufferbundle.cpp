#include "scarablib/opengl/bufferbundle.hpp"

BufferBundle::~BufferBundle() noexcept {
	// If was made using the manager
	if(this->vao_hash != 0) {
		VAOManager::get_instance().release_vao(vao_hash); // Early release of the manager
	// If not, but still check for the ID
	} else if(this->vao_id != 0) {
		glDeleteVertexArrays(1, &this->vao_id);
	}

	if(this->vbo != nullptr) {
		delete vbo;
	}

	if(this->ebo != nullptr) {
		delete ebo;
	}
}

