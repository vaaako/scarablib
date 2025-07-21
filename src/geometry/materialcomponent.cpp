#include "scarablib/components/materialcomponent.hpp"

MaterialComponent::TextureHandle::TextureHandle() noexcept
	: ptr(Texture::default_texture()) {}
MaterialComponent::TextureHandle::TextureHandle(Texture* texture) noexcept
	: ptr(texture ? std::shared_ptr<Texture>(texture) : Texture::default_texture()) {}
MaterialComponent::TextureHandle::TextureHandle(const Texture& texture) noexcept
	: ptr(std::make_shared<Texture>(texture)) {}
MaterialComponent::TextureHandle::TextureHandle(std::nullptr_t) noexcept
	: ptr(Texture::default_texture()) {}

MaterialComponent::~MaterialComponent() noexcept {
	if(this->texture_array) {
		delete this->texture_array;
	}
}
