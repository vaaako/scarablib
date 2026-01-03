#pragma once

#include "scarablib/gfx/texturebase.hpp"

// Texture object used for shapes (2D and 3D)
class Texture : public TextureBase {
	public:
		// Create a solid white texture
		Texture() noexcept;

		// Create a texture from an image file.
		// You can also flip the texture vertically or horizontally
		Texture(const char* path, const bool flip_v = false, const bool flip_h = false);

		// Make texture from image
		Texture(const Image& image);

		~Texture() noexcept = default;
};
