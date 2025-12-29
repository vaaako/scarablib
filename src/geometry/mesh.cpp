#include "scarablib/geometry/mesh.hpp"

Mesh::~Mesh() noexcept {
	if(this->physics) {
		delete this->physics;
	}
	if(this->bbox) {
		delete this->bbox;
	}
}

