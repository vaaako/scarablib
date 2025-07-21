#pragma once

#include "scarablib/gfx/texturebase.hpp"
#include <memory>

// Texture object used for shapes (2D and 3D)
class Texture : public TextureBase {
	public:
		// Create a solid white texture
		Texture() noexcept;

		// Create a texture from an image file.
		// You can also flip the texture vertically or horizontally
		Texture(const char* path, const bool flip_vertically = false, const bool flip_horizontally = false);

		// Uses pre-defined data to make a texture
		Texture(const uint8* data, const uint32 width, const uint32 height, const uint32 channels);

		~Texture() noexcept = default;

		// Returns a default solid white texture
		static std::shared_ptr<Texture> default_texture() noexcept {
			// I don't like data being statically allocated this way
			// Cant put inside IScene because MaterialComponent also uses this and
			// not always batch drawing is used
			static std::shared_ptr<Texture> def_tex = std::make_shared<Texture>();
			return def_tex;
		}
};
