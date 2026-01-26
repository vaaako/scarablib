#include "scarablib/components/materialcomponent.hpp"

Material::~Material() noexcept {
	if(this->texture_array) {
		delete this->texture_array;
	}
}
