#pragma once

#include "scarablib/gfx/texturebase.hpp"

// Texture object used for shapes (2D and 3D)
class Texture : public TextureBase {
	public:
		// Create a solid white texture
		Texture() noexcept;

		// Create a texture from an image file.
		// You can also flip the texture vertically or horizontally
		Texture(const char* path, const bool flip_vertically = false, const bool flip_horizontally = false);

		// Loads texture from raw memory data
		Texture(const uint8* data, const uint32 width, const uint32 height, const uint8 channels);

		~Texture() noexcept = default;
};
