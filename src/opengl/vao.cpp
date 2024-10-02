#include "scarablib/opengl/vao.hpp"
#include "scarablib/proper/log.hpp"

VAO::VAO() {
#ifdef SCARAB_DEBUG_VAO
	LOG_INFO_FN("VAO constructor");
#endif
	glGenVertexArrays(1, &this->id);
}

VAO::~VAO() {
	glDeleteVertexArrays(1, &this->id);
}
