#pragma once

#include "scarablib/types/color.hpp"
#include "scarablib/types/texture.hpp"

// TODO: when material.texture = nullptr automatically set to default texture

struct MaterialComponent {
	// Current color
	Color color = Colors::WHITE;
	// Current texture being used.
	// Use `remove_texture()` to change
	Texture* texture = &Texture::default_texture(); // Default texture
	// This will always be a shared pointer to other texture

	// Removes the texture
	inline void remove_texture() noexcept {
		this->texture = &Texture::default_texture();
	}
};
