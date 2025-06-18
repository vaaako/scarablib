#pragma once

#include "scarablib/types/color.hpp"
#include "scarablib/types/texture.hpp"

struct MaterialComponent {
	// Current color
	Color color      = Colors::WHITE;
	// Current texture being used.
	// Use `remove_texture()` to change.
	// Pass nullptr to revert to default texture
	Texture* texture = &Texture::default_texture(); // Default texture
	// This will always be a shared pointer to other texture

	// Sets a new texture.
	// If `texture` is nullptr, the default texture will be used instead
	inline void remove_texture() noexcept {
		this->texture = &Texture::default_texture();
	}

	// // Gets a reference to the current texture
	// inline Texture& get_texture() const noexcept {
	// 	return *this->texture;
	// }
};
