#include "scarablib/components/materialcomponent.hpp"

MaterialComponent::TextureHandle::TextureHandle() noexcept
	: ptr(Assets::default_texture()) {}
MaterialComponent::TextureHandle::TextureHandle(std::nullptr_t) noexcept
	: ptr(Assets::default_texture()) {}

MaterialComponent::~MaterialComponent() noexcept {
	if(this->texture_array) {
		delete this->texture_array;
	}
}
