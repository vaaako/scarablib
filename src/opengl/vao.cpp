#include "scarablib/opengl/vao.hpp"

VAO::VAO(const uint32 id) noexcept {
	if(id != 0) {
		this->id = id;
		return;
	}

	#ifdef SCARAB_DEBUG_VAO
	LOG_INFO_FN("VAO constructor");
	#endif
	glGenVertexArrays(1, &this->id);
}

VAO::~VAO() noexcept {
	glDeleteVertexArrays(1, &this->id);
}
