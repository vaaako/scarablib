#include "scarablib/opengl/vertexarraybuilder.hpp"

VertexArrayBuilder::~VertexArrayBuilder() noexcept {
	this->cleanup_attributes();
}
