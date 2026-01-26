#pragma once

#include "scarablib/gfx/texturebase.hpp"

// Texture object used for shapes (2D and 3D)
class Texture : public TextureBase {
	public:
		// Create a solid white texture
		Texture() noexcept;

		// Create a new texture out of an image
		Texture(const Image& image);

		Texture(const uint8* data, const uint32 width, const uint32 height, const uint8 channels);

		~Texture() noexcept = default;
};
